/* =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Extract harris corners coordinates from
 *    				an image and save it to a file
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

#include <iostream>
#include <stdio.h>
#include <list>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "libs/Filters.h"
#include "libs/HarrisCorner.h"

using namespace cv;
using namespace std;
using namespace features;

bool compPt(Point i,Point j)
{
	if(i.x==j.x)
		return (i.y < j.y);
	return (i.x<j.x);
}

void saveCorners(const char* fname, vector<Point> xv)
{
	ofstream myfile;
	myfile.open (fname, ios::out | ios::trunc);

	myfile<<xv.size()<<endl;

	vector<Point>::iterator it;
	for(it = xv.begin(); it!=xv.end(); ++it)
	{
		myfile<<it->x<<" "<<it->y<<" "<<endl;
	}
}

int main(int argc, char** argv)
{
	string imgpath, coord3d;

	if (argc != 3)
	{
		cout << " Usage: TsaiCalibration imgPath 3dcoordfile" << endl;
		cout << " Setting default to: TsaiCalibration img/tsai1.jpg tsai3d1.txt" << endl;

		imgpath = "img/tsai1.jpg";
		coord3d = "img/tsai3d1.txt";
	}
	else
	{
		imgpath = argv[1];
		coord3d = argv[2];
	}


	Mat image, gray, gauss, sx, sy, sobel, harris, harrisNMS;

	image = imread(imgpath, CV_LOAD_IMAGE_COLOR);   // Read the file
	Filters::colorToGray(image, gray);
	Filters::gaussian(gray, gauss, 1.0f);
	Filters::sobel(gauss, sx, sy, sobel);
	HarrisCorner::harris(sx, sy, harris, 0.06f, 5);
	//createDisplayImg("harris", harris);

	vector<Point> cornersH, cornersNMS;
	HarrisCorner::getCornerPoints(harris, cornersH, 0.3f);
	printf("Harris Corners L & %d \\\\\n", (int)cornersH.size());

	HarrisCorner::nonMaxSuppression(harris, harrisNMS, 17);
	HarrisCorner::getCornerPoints(harrisNMS, cornersNMS, 0.64f);
	printf("NMS Corners L & %d \\\\\n", (int)cornersNMS.size());
	HarrisCorner::drawCorners(image, cornersNMS);
	createDisplayImg("harrisNMS", image);
	//imwrite( "kuma.png", image );

	std::sort(cornersNMS.begin(), cornersNMS.end(), compPt);
	cout<<cornersNMS;
	saveCorners(coord3d.c_str(), cornersNMS);

	waitKey(0);
	return 0;
}
