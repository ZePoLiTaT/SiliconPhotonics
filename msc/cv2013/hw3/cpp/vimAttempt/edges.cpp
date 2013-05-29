/*
 * =====================================================================================
 *
 *       Filename:  edges.cpp
 *
 *    Description:  Utility functions for Sobel border detection and Harris edge
 *					detection
 *
 *        Version:  1.0
 *        Created:  19/05/13 07:13:33
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Tatiana López G (ZePoLiTaT), tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * =====================================================================================
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define rep(i,n) for(int i=0; i<(n); i++)
#define rep2(i,n) for(int i=0; i<=(n); i++)
#define repf(i,a,b) for(int i=(a); i<=(b); i++)
#define repb(i,a,b) for (int i=(a); i>=b; i--) 

#define GETELE(type,mat,xpos,ypos) *((type*)(mat.data+mat.step()*(ypos)+(xpos)*mat.elemSize))

using namespace cv;
using namespace std;

Mat* createGaussianKernel(float sigma, int WR)
{
	Mat kern = (Mat_<float>(WR*2,WR*2));
	float f = GETELE(float,kern,1,1)
	rep(i,WR){
	}

	return 0;
}
/* -----  end of function createGaussianKernel ----- */

void convolve (  )
{
	return ;
}	
/* -----  end of function convolve  ----- */
