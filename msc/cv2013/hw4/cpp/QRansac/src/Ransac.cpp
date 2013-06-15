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

using namespace std;
using namespace cv;

inline void printMat(Mat &m)
{
	cout<<"-----------------"<<endl;
	cout<<m(Rect(0,0,9,15))<<endl;
	cout<<"-----------------F";
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

	T = (Mat_<float>(3,3) << 1, 0, -tx,
							 0, 1, -ty,
							 0, 0,  1);
}

void getscale(const Mat &x, Mat &T)
{
	float d=0, s=0;
	int N = x.rows;

	for(int i=0; i<N; i++)			//Calculate the average distance to the centroid
	{
		const float* xP = x.ptr<float>(i);
		d += sqrt((xP[0] * xP[0]) + (xP[1] * xP[1]));
	}

	d /= N;

	s = sqrt(2) / d;				//Set a factor where the mean distance is sqrt(2):  d*s = sqrt(2)

	T = (Mat_<float>(3,3) <<  s, 0, 0,
						  	  0, s, 0,
							  0, 0, 1);

}

void normalize(Mat &x, Mat &T)
{
	Mat Ttrans, Tscale;

	getcentroid(x, Ttrans);			//Get the centroid of the points
	x = Ttrans*x.t();				//Apply the found transformation
	x = x.t();

	getscale(x, Tscale);			//Get the scale of the points
	x = Tscale*x.t();				//Apply the found transformation
	x = x.t();

	T = Tscale*Ttrans;				//The final T is given by first translating and then scaling
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

	SVD svd(A);
	Mat lastVt = svd.vt(Rect(0,8,9,1));
	H = lastVt.reshape(lastVt.channels(), 3);
}

void randomPerm(int N, int n, int* idx)
{
	bool found[n];
	for(int i=0; i<n; i++)	found[i]=false;					//mark all as free

	int i=0;
	while(i<n)
	{
		idx[i] = rand()%N;
			if(!found[idx[i]])	found[idx[i++]] = true;		//make sure there are not repeated elements
	}
}

void randomSample(Mat &xsa, Mat &xpsa, const Mat &x, const Mat &xp, int n)
{
	xsa.create(n,3,x.type());
	xpsa.create(n,3,x.type());

	int sampleIdx[n];
	randomPerm(xsa.rows, n, sampleIdx);

	for(int i=0; i<n; i++)
	{
		float *xsaP = xsa.ptr<float>( sampleIdx[i] );
		float *xpsaP = xpsa.ptr<float>( sampleIdx[i] );
		const float *xP = x.ptr<float>( sampleIdx[i] );
		const float *xpP = xp.ptr<float>( sampleIdx[i] );

		xsaP[0] = xP[0];
		xsaP[1] = xP[1];
		xsaP[2] = xP[2];

		xpsaP[0] = xpP[0];
		xpsaP[1] = xpP[1];
		xpsaP[2] = xpP[2];
	}
}

void getModel(Mat &H, const Mat &x, const Mat &xp)
{
	H.create(3,3,x.type());
	dlt(H, x, xp);
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

	return sqrt(xdif*xdif + ydif*ydif);
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
float transferError(const Mat &x, const Mat &xest)
{
	float d = 0;
	for(int i=0; i<x.rows; i++)
	{
		const float *xP = x.ptr<float>(i);
		const float *xestP = xest.ptr<float>(i);

		d += geometricDistance(	xP[0], xP[1], xP[2],
								xestP[0], xestP[1], xestP[2]);
	}

	return d;
}

void calculateSupport(Mat &H, const Mat &x, const Mat &xp)
{
	float symmTransfErr;					//Symmetric transfer error

	Mat xest(x.rows, x.cols, x.type());
	Mat xpest(xp.rows, xp.cols, xp.type());

	xpest = H*x.t();
	xest = H.inv()*xp.t();

	symmTransfErr = transferError(x, xest) + transferError(xp, xpest);
}

void RANSAC(Mat &x, Mat &xp, int n)
{
	int ntot = x.rows;						//Amount of data
	int N = numeric_limits<int>::max();		//Adaptative max-iteration limit
	float p = 0.99f;						//Desired probability of not finding an outlier in the next iteration
	float eeps = 0.20f;						//Desired probabilty of outliers
	float M = (1-eeps)*ntot;				//Model condition based on eeps

	Mat xsa, xpsa;							//Sampled pairs (x,xp)
	Mat H;									//Obtained Model

	int cont = 0;
	while (cont<N)
	{
		randomSample(xsa, xpsa, x, xp, n);	//1. Get a radom subsample of the data
		getModel(H, xsa, xpsa);				//2. Find a model based on such subsample
		calculateSupport(H, x, xp);			//3. Find the number of inliers based on the found model
	}
}

int main()
{
	srand (time(NULL));						//initialize random seed

	Mat x, xp;
	loadCorrespondences(x, xp);

	Mat T, Tp;
	normalize(x, T);
	normalize(xp, Tp);

	Mat Hn, H;
	dlt(Hn, x, xp);

	denormalize(H, Hn, Tp, T);

	cout<<endl<<"************ BEGIN"<<endl;
	cout<<H;
	cout<<endl<<"************ END"<<endl;

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
