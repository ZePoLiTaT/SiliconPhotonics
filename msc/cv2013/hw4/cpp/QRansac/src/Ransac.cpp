//============================================================================
// Name        : Ransac.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define EPSILON 1e-6						//Colinearity check epsilon
#define REQn	5							//Number of points required to calculate the H
#define Meps	0.22f						//desired epsilon
#define P		0.99f						//Desired probability of not finding an outiler
											//in the next iteration

using namespace std;
using namespace cv;

////////////////////////// UNIT TESTS ////////////////////////////
void colinearityTest();

////////////////////////// UTILITIES /////////////////////////////
inline void printMat(Mat &m)
{
	cout<<"-----------------"<<endl;
	cout<<m(Rect(0,0,9,15))<<endl;
	cout<<"-----------------F";
}

inline void createDisplayImg(string name, Mat& image)
{
	namedWindow(name, CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow(name, image);                   // Show our image inside it.
}

inline void createDisplayImg(string name, const Mat& image1, const Mat& image2, Mat &dst)
{
	dst.create(max(image1.rows, image2.rows), image1.cols + image2.cols, image1.type());

	//ROI of bigger image pointing to first half
	Mat dstImg1 = dst(Rect(0, 0, image1.cols, image1.rows));
	//ROI of bigger image pointing to second half
	Mat dstImg2 = dst(Rect(image1.cols, 0, image2.cols, image2.rows));

	image1.copyTo(dstImg1);
	image2.copyTo(dstImg2);

	createDisplayImg(name, dst);
}
/////////////////////////////////////////////////////////////////////////

bool loadImages(string fnameI1, string fnameI2, Mat &image1, Mat &image2)
{
	image1 = imread(fnameI1, CV_LOAD_IMAGE_COLOR);		// Read the file
	image2 = imread(fnameI2, CV_LOAD_IMAGE_COLOR);		// Read the file

	if (!image1.data || !image2.data)                 	// Check for invalid input
	{
		cout << "Could not open or find the image" << endl;
		return false;
	}

	return true;
}

bool loadCorrespondences(string fname, Mat &x, Mat &xp)
{
	int N;
	float p1x, p1y, p2x, p2y;

	FILE *file = fopen(fname.c_str(),"r");
	if (file == NULL)
	{
		cout << "Could not open correspondences file" << endl;
		return false;
	}


	fscanf(file, "%d\n", &N);

	x.create(N,3,CV_32F);
	xp.create(N,3,CV_32F);

	for(int i=0; i<N; i++)
	{
		float* xP = x.ptr<float>(i);
		float* xpP = xp.ptr<float>(i);

		fscanf(file, "%f %f %f %f\n", &p1x, &p1y, &p2x, &p2y);
		xP[0] = p1x;
		xP[1] = p1y;
		xP[2] = 1;

		xpP[0] = p2x;
		xpP[1] = p2y;
		xpP[2] = 1;
	};

	fclose (file);
	return true;
}

void getcentroid(const Mat &x, Mat &T)
{
	T.create(3,3,CV_32F);

	float tx=0, ty=0;
	int N = x.rows;

	for(int i=0; i<N; i++)			//Calculate the centroid of the points
	{
		const float* xP = x.ptr<float>(i);
		tx += xP[0];
		ty += xP[1];
	}

	tx /= N;
	ty /= N;

	T.at<float>(0,0)=1;
	T.at<float>(0,1)=0;
	T.at<float>(0,2)=-tx;
	T.at<float>(1,0)=0;
	T.at<float>(1,1)=1;
	T.at<float>(1,2)=-ty;
	T.at<float>(2,0)=0;
	T.at<float>(2,1)=0;
	T.at<float>(2,2)=1;
}

void getscale(const Mat &x, Mat &Tscale, const Mat &Ttrans)
{
	Tscale.create(3,3,CV_32F);

	float d=0, s=0, tmpx, tmpy;
	int N = x.rows;

	float meanx = -Ttrans.at<float>(0,2);
	float meany = -Ttrans.at<float>(1,2);

	for(int i=0; i<N; i++)			//Calculate the average distance to the centroid
	{
		const float* xP = x.ptr<float>(i);

		tmpx = xP[0] - meanx;
		tmpy = xP[1] - meany;
		d += sqrt( (tmpx * tmpx) + (tmpy * tmpy) ) ;
	}

	d /= N;

	s = sqrt(2) / d;				//Set a factor where the mean distance is sqrt(2):  d*s = sqrt(2)

	Tscale.at<float>(0,0)=s;
	Tscale.at<float>(0,1)=0;
	Tscale.at<float>(0,2)=0;
	Tscale.at<float>(1,0)=0;
	Tscale.at<float>(1,1)=s;
	Tscale.at<float>(1,2)=0;
	Tscale.at<float>(2,0)=0;
	Tscale.at<float>(2,1)=0;
	Tscale.at<float>(2,2)=1;
}

void normalizze(Mat &x, Mat &T)
{
	Mat Ttrans, Tscale;

	getcentroid(x, Ttrans);			//Get the centroid of the points
	getscale(x, Tscale, Ttrans);	//Get the scale of the points

	T = Tscale*Ttrans;				//The final T is given by first translating and then scaling

	x = T * x.t();					//Apply the found transformation to normalize the data
	x = x.t();
}

void denormalize(Mat &H, const Mat &Hn, const Mat &Tp, const Mat &T)
{
	H.create(Hn.rows, Hn.cols, Hn.type());

	H = Tp.inv()*Hn*T;
	H /= H.at<float>(2,2);
}

/*
 * Create the A matrix with the following rows:
 *
 * Row 1: [0, 0, 0,    -wip * xi',   yip * xi']
 * Row 2: [wip * xi',    0, 0, 0,   -xip * xi']
 */
void dlt(Mat &H, const Mat &x,const  Mat &xp)
{
	Mat A(2*x.rows, 9, x.type());

	for(int i=0; i<x.rows; i++)
	{
		float *AP = A.ptr<float>(2*i);
		const float* xP = x.ptr<float>(i);		//xi =xpP[0], yi =xpP[1], wi =xpP[2]
		const float* xpP = xp.ptr<float>(i);	//xip=xpP[0], yip=xpP[1], wip=xpP[2]

		//  		Row 1					Row 2
		AP[0] = 0;					AP[9]  = ( xpP[2]*xP[0]);
		AP[1] = 0;					AP[10] = ( xpP[2]*xP[1]);
		AP[2] = 0;					AP[11] = ( xpP[2]*xP[2]);
		AP[3] = (-xpP[2]*xP[0]);	AP[12] = 0;
		AP[4] = (-xpP[2]*xP[1]);	AP[13] = 0;
		AP[5] = (-xpP[2]*xP[2]);	AP[14] = 0;
		AP[6] = ( xpP[1]*xP[0]);	AP[15] = (-xpP[0]*xP[0]);
		AP[7] = ( xpP[1]*xP[1]);	AP[16] = (-xpP[0]*xP[1]);
		AP[8] = ( xpP[1]*xP[2]);	AP[17] = (-xpP[0]*xP[2]);
	};

	SVD svd(A.t()*A);
	Mat lastVt = svd.vt(Rect(0,8,9,1));
	H = lastVt.reshape(lastVt.channels(), 3);
}

bool colineality(Mat &pts)
{
	int valIndx = pts.rows;
	float val;
	Mat p1, p2, p3, l, tmp;

	for(int i=0; i<valIndx; i++)
	{
		for(int j=i+1; j<valIndx; j++)
		{
			for(int k=j+1; k<valIndx; k++)
			{
				p1 = pts(Rect(0, i, 3, 1));	//Get the points to validate
				p2 = pts(Rect(0, j, 3, 1));
				p3 = pts(Rect(0, k ,3, 1));

				l = p1.t().cross(p2.t());				//Get the line formed by p1 and p2
				tmp = p3 * l;		//If p3 is on the line, this result will be close to 0
				val = tmp.at<float>(0, 0);

				if (abs(val) < EPSILON)
					return true;
			}
		}
	}

	return false;
}

void randomPerm(int N, int n, int* idx)
{
	bool found[N];
	for(int i=0; i<N; i++)	found[i]=false;				//mark all as free

	int i=0;
	while(i<n)
	{
		idx[i] = rand()%N;
		if(!found[idx[i]])
			found[idx[i++]] = true;		//make sure there are not repeated elements
	}
}

void randomSample(Mat &xsa, Mat &xpsa, const Mat &x, const Mat &xp)
{
	xsa.create(REQn,3,x.type());
	xpsa.create(REQn,3,x.type());

	int pos;
	int sampleIdx[REQn];
	bool arecolinear;

	do
	{
		randomPerm(x.rows, REQn, sampleIdx);			//Get 5 random points without repeating

		for(int i=0; i<REQn; i++)						//Extract the data
		{
			pos = sampleIdx[i];

			float *xsaP = xsa.ptr<float>(i);
			float *xpsaP = xpsa.ptr<float>(i);
			const float *xP = x.ptr<float>(pos);
			const float *xpP = xp.ptr<float>(pos);

			xsaP[0] = xP[0];
			xsaP[1] = xP[1];
			xsaP[2] = xP[2];

			xpsaP[0] = xpP[0];
			xpsaP[1] = xpP[1];
			xpsaP[2] = xpP[2];
		}

		arecolinear = colineality(xsa);
	}while(arecolinear);
}

void getModel(Mat &H, Mat &x, Mat &xp)
{
	H.create(3,3,x.type());

	Mat T, Tp, Hn;

	normalizze(x, T);
	normalizze(xp, Tp);

	dlt(Hn, x, xp);

	denormalize(H, Hn, Tp, T);
}

/*
 * Calculate the geometric distance between a pair of homogeneous points:
 *
 * 	d(X, X') = sqrt( (xi/wi - xip/wip)^2 + (yi/wi - yip/wip)^2 )
 *
 * 	Where	X = [xi, yi, wi]'  and  X' = [xip, yip, wip]'
 */
inline float geometricDistance(float xi,float  yi,float wi,float xip,float yip,float wip)
{
	float xdif, ydif;

	xdif = xi/wi - xip/wip;
	ydif = yi/wi - yip/wip;

	return xdif*xdif + ydif*ydif;
}


/*
 * Measures the correction necessary to make the measurements
 * in one image (2nd image) in order to get a perfectly matching
 * points. The formula is given by:
 *
 * \sum_i d(X, Xest')^2
 *
 * Where d is the geometric distance.
 */
float calculateSupport(bool *inlset, float *inlcount, const Mat &H, const Mat &x, const Mat &xp, float thrs)
{
	float d1, d2;								//Geometric distances
	float symmTransfErr;						//Symmetric transfer error
	float accSymTransfErr = FLT_MAX;			//Accumulated symmetric transfer error of the inliers

	float min = FLT_MAX;

	Mat xest(x.rows, x.cols, x.type());
	Mat xpest(xp.rows, xp.cols, xp.type());

	xpest = H*x.t();							//Calulate the estimated x' from Hx
	xpest = xpest.t();

	xest = H.inv()*xp.t();						//Calculate the estimated x from inv(H)x'
	xest = xest.t();

	(*inlcount) = 0;							//Reset the inliers counter
	for (int i = 0; i < x.rows; i++)
	{
		const float *xP = x.ptr<float>(i);
		const float *xestP = xest.ptr<float>(i);

		const float *xpP = xp.ptr<float>(i);
		const float *xpestP = xpest.ptr<float>(i);

		d1 = geometricDistance(xP[0], xP[1], xP[2], xestP[0], xestP[1], xestP[2]);
		d2 = geometricDistance(xpP[0], xpP[1], xpP[2], xpestP[0], xpestP[1], xpestP[2]);

		symmTransfErr = d1 + d2;				//Symmetric transfer error is the sum of the 2 geometric distances
		if(symmTransfErr<min)
			min = symmTransfErr;

		if(symmTransfErr < thrs)				//If the symmetric transfer error is less than 5.99var
		{										//then it's an inlier
			(*inlcount)++;
			inlset[i] = true;
			accSymTransfErr += symmTransfErr;
		}
		else
			inlset[i] = false;
	}

	//cout<<"Minimo: "<<min;
	return accSymTransfErr;
}

void extractPoints(Mat &xnew, bool *idx, int count, const Mat &x)
{
	xnew.create(count,3,x.type());

	int xnewidx = 0;
	for(int i=0; i<x.rows; i++)
	{
		if(idx[i])
		{
			float *xnewP = xnew.ptr<float>(xnewidx++);
			const float *xP = x.ptr<float>(i);

			xnewP[0] = xP[0];
			xnewP[1] = xP[1];
			xnewP[2] = xP[2];
		}
	}
}

void RANSAC(Mat &H, bool *maxinlset, const Mat &x,const Mat &xp, float thrs)
{
	int ntot = x.rows;						//Amount of data
	int Ntmp, N = 10000;						//Adaptative max-iteration limit
	//float M = (1-Meps)*ntot;				//Model condition based on eeps
	float eps = 0.5;						//Probability of finding an outlier
	float inl, maxinl = 0; 			//Counter of inliers and maximum found inliers
	float symE, minSymE = FLT_MAX;			//Accumulated symetric error and the minimun found
	bool inlset[ntot];						//List of indexes with the inliers and best set of inliers were found
	int cont = 0;							//Iteration counter

	Mat xsa, xpsa;							//Sampled pairs (x,xp)
	Mat xin, xpin;							//Points that are inliers (used to reestimate the model only)

	cout<<"Threshold: "<<thrs<<endl;
	while (cont<N)
	{
		randomSample(xsa, xpsa, x, xp);							//1. Get a radom subsample of the data
		getModel(H, xsa, xpsa);									//2. Find a model based on such subsample
		symE = calculateSupport(inlset, &inl, H, x, xp, thrs);	//3. Find the number of inliers based on the found model

		if(inl > maxinl || (inl==maxinl && symE<minSymE))		//4. If a new set with more inliers of a best symTransfer error
		{														//	 is found then, save this new best solution
			maxinl = inl;
			minSymE = symE;
			copy(inlset, inlset+ntot, maxinlset);

			eps = 1 - inl/ntot;									//5. Update the probability of finding outliers
			Ntmp = log(1-P)/log(1-pow(1-eps, REQn));			//6. Re-calculate N adaptatively with the new eps value
			N = (Ntmp>=0 && Ntmp<N)?Ntmp:N;
			cout<<" ... Updating N: "<<N<<" with: "<<inl<<" inliers"<<endl;
		}

		cont++;
	}


	extractPoints(xin,maxinlset,maxinl, x);
	extractPoints(xpin,maxinlset,maxinl, xp);
	getModel(H, xin, xpin);										//7. Re-estimate the model only with the inliers

	cout<<"Iterations: "<<cont<<endl;
	cout<<"OUTLIERS FOUND: "<<ntot-maxinl<<endl;
	cout<<"INLIERS FOUND: "<<maxinl<<endl;


}

void draw(const Mat &img1, const Mat &x, const Mat &img2, const Mat &xp, const bool *inliers, Mat& dst)
{
	dst.create(max(img1.rows, img2.rows), img1.cols + img2.cols, img1.type());

	Mat dstImg1 = dst(Rect(0, 0, img1.cols, img1.rows));			//ROI of bigger image pointing to first half
	Mat dstImg2 = dst(Rect(img1.cols, 0, img2.cols, img2.rows));	//ROI of bigger image pointing to second half

	img1.copyTo(dstImg1);
	img2.copyTo(dstImg2);

	Scalar colorOUT(0, 0, 255);										//Color for outliers
	Scalar colorIN(0, 255, 0);										//Color for inliers
	Scalar *color;

	for(int i=0; i<x.rows; i++)
	{
		const float* xP = x.ptr<float>(i);
		const float* xpP = xp.ptr<float>(i);

		color = inliers[i] ? &colorIN : &colorOUT;

		line(dst, Point(xP[0], xP[1]), Point(xpP[0]+img1.cols, xpP[1]), *color, 1, CV_AA);
	}
}

void draw(Mat &img2, const Mat &Hx, const Mat &xp, const bool *inliers)
{
	Scalar colorCenter(128, 128, 128);
	Scalar colorXp(0, 255, 255);
	Scalar colorHxIN(0, 255, 0);
	Scalar colorHxOUT(0, 0, 255);
	Scalar *color;

	Point centerP2, centerP1;

	for(int i=0; i<Hx.rows; i++)
	{
		const float* HxP = Hx.ptr<float>(i);
		const float* xpP = xp.ptr<float>(i);

		centerP1 = Point(xpP[0]/xpP[2], xpP[1]/xpP[2]);
		centerP2 = Point(HxP[0]/HxP[2], HxP[1]/HxP[2]);
		color = inliers[i] ? &colorHxIN : &colorHxOUT;

		circle(img2, centerP1 ,5, colorXp, 2, CV_AA);
		if(!inliers[i])
			circle(img2, centerP2, 3, *color, 2, CV_AA);

		//line(img2, centerP2, centerP2, colorCenter, 2, CV_AA);
		//putText(img2, "+", Point(HxP[0]/HxP[2]-7, HxP[1]/HxP[2]+5), 0, 0.5, colorHx, 2, CV_AA);
	}

	for(int i=0; i<Hx.rows; i++)
	{
		const float* HxP = Hx.ptr<float>(i);

		centerP2 = Point(HxP[0]/HxP[2], HxP[1]/HxP[2]);
		color = inliers[i] ? &colorHxIN : &colorHxOUT;
		if(inliers[i])
			circle(img2, centerP2, 2, *color, 2, CV_AA);
	}
}

int main(int argc, char** argv)
{
	srand(time(NULL));								//initialize random seed

	Mat image1, image2;
	string fnameI1, fnameI2, file;
	float thrs;

	if (argc != 5)
		{
			fnameI1 = "img/benevolentAnnika.jpg";
			fnameI2 = "img/benevolentAnnikaRot.jpg";
			file = "correspondences1.txt";
			thrs = 0.35f;

			printf("Usage: Ransac PathImg1 PathImg2 PathCorrespondencesFile threshold\n");
			printf(" Setting default to: Ransac img/benevolentAnnika.jpg img/benevolentAnnikaRot.jpg correspondences1.txt 0.35f\n");
		}
		else
		{
			fnameI1 = argv[1];
			fnameI2 = argv[2];
			file = argv[3];
			thrs = atof(argv[4]);

			printf(" Loaded: Ransac %s %s %s %f\n", fnameI1.c_str(), fnameI2.c_str(), file.c_str(), thrs);
		}

	if( !loadImages(fnameI1, fnameI2, image1, image2) )
		return -1;

	Mat x, xp;
	if( !loadCorrespondences(file, x, xp))
		return -1;

	Mat H(3,3,CV_32F);
	bool inliers[x.rows];
	RANSAC(H, inliers, x, xp, thrs);
	//Mat xdlt = x.clone(), xpdlt = xp.clone(); getModel(H, xdlt, xpdlt);

	Mat bigimg;
	draw(image1, x, image2, xp, inliers, bigimg);
	createDisplayImg("Outliers", bigimg);

	Mat Hx = H * x.t();
	Hx = Hx.t();
	draw(image2, Hx, xp, inliers);
	createDisplayImg("Proyection Hx,Xp", image2);

	Mat HiXp = H.inv() * xp.t();
	HiXp = HiXp.t();
	draw(image1, HiXp, x, inliers);
	createDisplayImg("Proyection inv(H)Xp,X", image1);

	waitKey(0);

	/*imwrite( "../../doc/figs/DLT.png", bigimg );
	imwrite( "../../doc/figs/DLT1.png", image2 );
	imwrite( "../../doc/figs/DLT2.png", image1 );*/

	return 0;
}

////////////////////////// UNIT TESTS ////////////////////////////
void colinearityTest()
{
	Mat test = (Mat_<float >(4,3) << 0.0f, 1.0f, 1.0f,
									 1.0f, -5.0f,1.0f,
									 0.0f, 2.0f, 1.0f,
									 0.0f, -3.0f, 1.0f);
	assert(colineality(test));
}
//////////////////////////////////////////////////////////////////
