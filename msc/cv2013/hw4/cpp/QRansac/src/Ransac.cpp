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


#define REQn	5							//Number of points required to calculate the H
#define INTHRSH	1000.0f 	 					//sqrt(5.99)*var
#define Meps	0.22f						//desired epsilon
#define P		0.99f						//Desired probability of not finding an outiler
											//in the next iteration

using namespace std;
using namespace cv;

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

void loadCorrespondences(Mat &x, Mat &xp)
{
	int N;
	float p1x, p1y, p2x, p2y;

	scanf("%d\n", &N);

	x.create(N,3,CV_32F);
	xp.create(N,3,CV_32F);

	for(int i=0; i<N; i++)
	{
		float* xP = x.ptr<float>(i);
		float* xpP = xp.ptr<float>(i);

		scanf("%f %f %f %f\n", &p1x, &p1y, &p2x, &p2y);
		xP[0] = p1x;
		xP[1] = p1y;
		xP[2] = 1;

		xpP[0] = p2x;
		xpP[1] = p2y;
		xpP[2] = 1;
	};
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

void randomPerm(int N, int n, int* idx)
{
	bool found[N];
	for(int i=0; i<N; i++)	found[i]=false;				//mark all as free

	cout<<"Randperm: ";
	int i=0;
	while(i<n)
	{
		//idx[i] = 1+i++;
		//cout<<idx[i-1]<<" ";
		idx[i] = rand()%N;
		if(!found[idx[i]])
		{
			cout<<idx[i]<<" ";
			found[idx[i++]] = true;		//make sure there are not repeated elements
		}
	}
	cout<<endl;
}

void randomSample(Mat &xsa, Mat &xpsa, const Mat &x, const Mat &xp)
{
	xsa.create(REQn,3,x.type());
	xpsa.create(REQn,3,x.type());

	int pos;
	int sampleIdx[REQn];
	randomPerm(x.rows, REQn, sampleIdx);

	for(int i=0; i<REQn; i++)
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
float calculateSupport(bool *inlset, float *inlcount, const Mat &H, const Mat &x, const Mat &xp)
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

		if(symmTransfErr < INTHRSH)				//If the symmetric transfer error is less than 5.99var
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

void RANSAC(Mat &H, bool *maxinlset, const Mat &x,const Mat &xp)
{
	int ntot = x.rows;						//Amount of data
	int N = 200;							//Adaptative max-iteration limit
	//float M = (1-Meps)*ntot;				//Model condition based on eeps
	float eps = 0.5;						//Probability of finding an outlier
	float inl, maxinl = FLT_MIN; 			//Counter of inliers and maximum found inliers
	float symE, minSymE = FLT_MAX;			//Accumulated symetric error and the minimun found
	bool inlset[ntot];						//List of indexes with the inliers and best set of inliers were found
	int cont = 0;							//Iteration counter

	Mat xsa, xpsa;							//Sampled pairs (x,xp)
	Mat xin, xpin;							//Points that are inliers (used to reestimate the model only)

	while (cont<N)
	{
		randomSample(xsa, xpsa, x, xp);							//1. Get a radom subsample of the data
		getModel(H, xsa, xpsa);									//2. Find a model based on such subsample
		symE = calculateSupport(inlset, &inl, H, x, xp);		//3. Find the number of inliers based on the found model

		if(inl > maxinl || (inl==maxinl && symE<minSymE))		//4. If a new set with more inliers of a best symTransfer error
		{														//	 is found then, save this new best solution
			maxinl = inl;
			minSymE = symE;
			copy(inlset, inlset+ntot, maxinlset);

			eps = 1 - inl/ntot;									//5. Update the probability of finding outliers
			N = log(1-P)/log(1-pow(1-eps, REQn));				//6. Re-calculate N adaptatively with the new eps value
			cout<<" ... Updating N: "<<N<<endl;
		}

		cont++;
	}


	extractPoints(xin,maxinlset,maxinl, x);
	extractPoints(xpin,maxinlset,maxinl, xp);
	getModel(H, xin, xpin);										//7. Re-estimate the model only with the inliers

	cout<<"INLIERS FOUND: "<<maxinl<<endl;
	cout<<endl<<"************ BEGIN in: "<<maxinl<<endl;
	cout<<H;
	cout<<endl<<"************ END"<<endl;

	cout<<"TOTAL Iterations: "<<cont<<endl;

}

void plot(const Mat &img1, const Mat &x, const Mat &img2, const Mat &xp, const bool *inliers, Mat& dst)
{
	dst.create(max(img1.rows, img2.rows), img1.cols + img2.cols, img1.type());

	//ROI of bigger image pointing to first half
	Mat dstImg1 = dst(Rect(0, 0, img1.cols, img1.rows));
	//ROI of bigger image pointing to second half
	Mat dstImg2 = dst(Rect(img1.cols, 0, img2.cols, img2.rows));

	img1.copyTo(dstImg1);
	img2.copyTo(dstImg2);

	//Color for inliers
	Scalar colorOUT(0, 0, 255);
	Scalar colorIN(0, 255, 0);
	Scalar *color;

	for(int i=0; i<x.rows; i++)
	{
		const float* xP = x.ptr<float>(i);
		const float* xpP = xp.ptr<float>(i);

		color = inliers[i] ? &colorIN : &colorOUT;

		line(dst, Point(xP[0], xP[1]), Point(xpP[0]+img1.cols, xpP[1]), *color, 1, CV_AA);
	}
}

int main()
{
	srand(time(NULL));								//initialize random seed

	Mat image, image1, image2;
	/*string fnameI1 = "img/benevolentAnnika.jpg";
	string fnameI2 = "img/benevolentAnnikaRot.jpg";*/
	/*string fnameI1 = "img/puregeometryRomanowsky.png";
	string fnameI2 = "img/puregeometryRomanowskyPersp.png";*/
	string fnameI1 = "img/foto1.jpg";
		string fnameI2 = "img/foto2.jpg";

	image1 = imread(fnameI1, CV_LOAD_IMAGE_COLOR);	// Read the file
	image2 = imread(fnameI2, CV_LOAD_IMAGE_COLOR);	// Read the file

	if (!image1.data || !image2.data)                 // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat x, xp;
	loadCorrespondences(x, xp);

	bool inliers[x.rows];
	Mat H(3,3,CV_32F);
	RANSAC(H, inliers, x, xp);

	Mat bigimg;
	plot(image1, x, image2, xp, inliers, bigimg);
	createDisplayImg("Plot", bigimg);

	/*Mat T, Tp;
	normalize(x, T);
	normalize(xp, Tp);

	Mat Hn, H;
	dlt(Hn, x, xp);

	denormalize(H, Hn, Tp, T);

	cout<<endl<<"************ BEGIN"<<endl;
	cout<<H;
	cout<<endl<<"************ END"<<endl;
	 */
	waitKey(0);
	return 0;
}

/*	Mat T = (Mat_<float>(3,3) <<  2, 1, 8,
						  	  	  5, 9, 5,
						  	  	  0, -2, 9);
	SVD svd(T);
	cout<<"u: "<<svd.u<<endl;
	cout<<"w: "<<svd.w<<endl;
	cout<<"vt: "<<svd.vt<<endl;
*/
