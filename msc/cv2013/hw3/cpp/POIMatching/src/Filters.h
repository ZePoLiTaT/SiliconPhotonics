/* =====================================================================================
 *
 *       Filename:  Edges.h
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  May 24, 2013
 *       Compiler:  g++
 *
 *         Author:  Tatiana López G (ZePoLiTaT) 
 *  	     Mail:  tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * ===================================================================================*/

#ifndef EDGES_H_
#define EDGES_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <map>

#include "POICompareMethods.h"

using namespace cv;
using namespace std;

class Filters
{
private:
	static float applyKernel(const Mat& img, int r, int c, float* kernel, int WSize);
public:
	Filters();
	virtual ~Filters();

	//Utility functions
	static int windowSize(float stdev);
	static void discreteGaussianKernel(float* kernel, float var, int WSize);
	static void sobelKernel(float* kernelX, float* kernelY, int WSize);
	static void magnitude(const Mat& mx, const Mat& my, Mat& mm);
	static void convolution(const Mat& src, Mat &dst, float* kernel, int WSize);

	//Filters
	static void colorToGray(const Mat& src, Mat& dst);
	static void gaussian(const Mat& src, Mat& dst, float stdev);
	static void sobel(const Mat& src, Mat& sx, Mat& sy, Mat& s, int WSize);
};

#endif /* EDGES_H_ */
