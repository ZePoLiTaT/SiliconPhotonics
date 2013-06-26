/* =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Camera Calibration with Tsai's Pattern
 *    				CV-Class
 *
 *        Version:  0.0.1
 *        Created:  Jun 22, 2013
 *       Compiler:  g++
 *
 *         Author:  Tatiana López G (ZePoLiTaT)
 *  	     Mail:  tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * ===================================================================================*/

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

bool loadCorrespondences(string fname, Mat &x, Mat &xp)
{
	int N;
	float p2dx, p2dy, p3dx, p3dy, p3dz;

	FILE *file = fopen(fname.c_str(),"r");
	if (file == NULL) return false;

	fscanf(file, "%d\n", &N);

	x.create(N,3,CV_32F);
	xp.create(N,4,CV_32F);

	for(int i=0; i<N; i++)
	{
		float* xP = x.ptr<float>(i);
		float* xpP = xp.ptr<float>(i);

		fscanf(file, "%f %f %f %f %f\n", &p2dx, &p2dy, &p3dx, &p3dy, &p3dz);
		xP[0] = p2dx;
		xP[1] = p2dy;
		xP[2] = 1;

		xpP[0] = p3dx;
		xpP[1] = p3dy;
		xpP[2] = p3dz;
		xpP[3] = 1;
	};

	return true;
}

void getcentroid(const Mat &x, Mat &T)
{
	int DIMS = x.cols;
	T.create(DIMS,DIMS,CV_32F);


	float t[DIMS];
	int N = x.rows;

	for(int i=0; i<N; i++)			//Calculate the centroid of the points
	{
		const float* xP = x.ptr<float>(i);
		for(int d=0; d<DIMS; d++)
		{
			t[d] += xP[d];
		}
	}

	T = Mat::eye(DIMS,DIMS, CV_32F);
	for(int d=0; d<DIMS-1; d++)
	{
		T.at<float>(d, DIMS-1) = -t[d]/N;
	}
}

void getscale(const Mat &x, Mat &Tscale, const Mat &Ttrans)
{
	int DIMS = x.cols;
	Tscale.create(DIMS,DIMS,CV_32F);

	float dist=0, s=0, acc;
	int N = x.rows;

	float mean[DIMS];
	for(int d=0; d<DIMS-1; d++)
	{
		mean[d] = -Ttrans.at<float>(d,DIMS-1);
	}

	for(int i=0; i<N; i++)			//Calculate the average distance to the centroid
	{
		acc = 0;
		const float* xP = x.ptr<float>(i);
		for(int d=0; d<DIMS-1; d++)
		{
			acc += (xP[d] - mean[d]) * (xP[d] - mean[d]);
		}
		dist += sqrt(acc);
	}

	dist /= N;
	s = sqrt(DIMS-1) / dist;				//Set a factor where the mean distance is sqrt(2):  d*s = sqrt(2)

	Tscale = Mat::eye(DIMS,DIMS, CV_32F);
	for(int d=0; d<DIMS-1; d++)
	{
		Tscale.at<float>(d, d) = s;
	}
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

void denormalize(Mat &H, const Mat &Hn, const Mat &T, const Mat &Tp)
{
	H.create(Hn.rows, Hn.cols, Hn.type());

	H = T.inv()*Hn*Tp;
	H /= H.at<float>(H.rows-1, H.cols-1);
}

/*
 * Create the A matrix with the following rows:
 *
 * Row 1: [0, 0, 0,    -wip * xi',   yip * xi']
 * Row 2: [wip * xi',    0, 0, 0,   -xip * xi']
 */
void dlt(Mat &H, const Mat &x,const  Mat &xp)
{
	int DIMS = xp.cols;
	Mat A(2*xp.rows, 3*DIMS, x.type());

	for(int i=0; i<x.rows; i++)
	{
		float *AP = A.ptr<float>(2*i);
		const float* xP = x.ptr<float>(i);		//xi =xpP[0], yi =xpP[1], wi =xpP[2]
		const float* xpP = xp.ptr<float>(i);	//xip=xpP[0], yip=xpP[1], wip=xpP[2]

		int idx = 0;
		///////////////////// ROW 1 /////////////////////////
		for(int d=0; d<DIMS; d++)				// 0'
			AP[idx++] = 0;
		for(int d=0; d<DIMS; d++)				// -wi*Xi'
			AP[idx++] = -xP[2]*xpP[d];
		for(int d=0; d<DIMS; d++)				// -yi*Xi'
			AP[idx++] = xP[1]*xpP[d];
		///////////////////// ROW 2 /////////////////////////
		for(int d=0; d<DIMS; d++)				// wiXi'
			AP[idx++] = xP[2]*xpP[d];
		for(int d=0; d<DIMS; d++)				// 0'
			AP[idx++] = 0;
		for(int d=0; d<DIMS; d++)				// -xi*Xi'
			AP[idx++] = -xP[0]*xpP[d];
	};

	SVD svd(A.t()*A);
	Mat lastVt = svd.vt(Rect(0,3*DIMS-1,3*DIMS,1));
	H = lastVt.reshape(lastVt.channels(), 3);
}

void getModel(Mat &H, Mat &x, Mat &xp)
{
	H.create(3,3,x.type());

	Mat T, Tp, Hn;

	normalizze(x, T);
	normalizze(xp, Tp);

	dlt(Hn, x, xp);

	denormalize(H, Hn, T, Tp);
}

int main(int argc, char** argv)
{
	string imgpath, coord3d;

	if (argc != 3)
	{
		cout << " Usage: TsaiCalibration imgPath 3dcoordfile" << endl;
		cout << " Setting default to: TsaiCalibration img/tsai1.jpg tsai3d1.txt" << endl;

		imgpath = "img/tsai1.jpg";
		coord3d = "tsai3d1.txt";
	}
	else
	{
		imgpath = argv[1];
		coord3d = argv[2];
	}

	Mat x, xp;

	if(!loadCorrespondences(coord3d, x, xp))
	{
		cout<<"There was a problem loading "<<coord3d<<" file."<<endl;
		return -1;
	}

	Mat P;
	getModel(P,x,xp);						//Find the P matrix

	Mat xpt = (Mat_<float>(1,4) <<  5, 0, 5, 1);
	Mat xt = (P*xpt.t()).t();
	cout<<xt;

	waitKey(0);
	return 0;
}

/*	Mat x = (Mat_<float>(4,3) <<  2, 1, 1,
								  5, 9, 1,
								  0, -2, 1,
								  1, 2, 1);
	Mat xp = (Mat_<float>(3,4) << 2, 1, 8, 1,
								  5, 9, 5, 1,
								  0, -2, 9, 1);

	Mat T;
	normalizze(xp, T);
*/
