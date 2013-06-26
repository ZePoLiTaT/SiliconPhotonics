/* =====================================================================================
 *
 *       Filename:  Harris.h
 *
 *    Description:  
 *
 *        Version:  0.0.1
 *        Created:  May 29, 2013
 *       Compiler:  g++
 *
 *         Author:  Tatiana López G (ZePoLiTaT) 
 *  	     Mail:  tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * ===================================================================================*/

#ifndef HARRIS_H_
#define HARRIS_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <math.h>

#include "Utils.h"
#include "Filters.h"

using namespace cv;
using namespace std;

namespace features
{

class HarrisCorner
{
private:
	/*int WHarrisSize;
	int WNMSSize;
	float k;*/
public:
	HarrisCorner();

	//void Harris::calibrate(int WHarrisSize, int WNMSSize, float k);
	static void harris(const Mat& dx, const Mat& dy, Mat& dst, float k, int WSize);
	static void nonMaxSuppression(Mat& mat, Mat &dst, int WSize);
	static void getCornerPoints(Mat& mat, vector<Point>& corners, mtype threshold);
	static void drawCorners(Mat &img, vector<Point>& corners, int radius=2);
};

} /* namespace poisim */
#endif /* HARRIS_H_ */
