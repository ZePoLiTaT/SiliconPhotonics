/* =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  Sobel edge and Harris corner detectors
 *    				CV-Class
 *
 *        Version:  0.0.1
 *        Created:  May 23, 2013
 *       Compiler:  g++
 *
 *         Author:  Tatiana López G (ZePoLiTaT)
 *  	     Mail:  tatiana@sirius.utp.edu.co
 *        Company:  Grupo de Investigación Sirius
 *
 * ===================================================================================*/

#include "Filters.h"
#include "HarrisCorner.h"
#include "POICorrespondence.h"

using namespace cv;
using namespace std;
using namespace features;
using namespace poi;

int main(int argc, char** argv)
{
	string fname;
	if (argc != 3)
	{
		cout << " Usage: main Image1 Image2" << endl;
		return -1;
	}

	Mat image1, image2;
	image1 = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file
	image2 = imread(argv[2], CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image1.data || !image2.data)                                // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	//createDisplayImg("Original Image", image1);
	//createDisplayImg("Original Image", image2);

	// ----------------- Convert Gray Scale -------------------- //
	Mat grayImg1, grayImg2;
	Filters::colorToGray(image1, grayImg1);
	Filters::colorToGray(image2, grayImg2);
	//createDisplayImg("Gray Image", grayImg1);

	// ----------------- Smooth with Gaussian Kernel -------------------- //

	float stdev = 1;
	Mat gausImg1, gausImg2;
	Filters::gaussian(grayImg1, gausImg1, stdev);
	Filters::gaussian(grayImg2, gausImg2, stdev);

	createDisplayImg("Gaussian Blur Image 1", gausImg1);
	createDisplayImg("Gaussian Blur Image 2", gausImg2);

	// ----------------- Detect Edges with Sobel -------------------- //
	Mat sIx1, sIy1, sobel1;
	Mat sIx2, sIy2, sobel2;
	Filters::sobel(gausImg1, sIx1, sIy1, sobel1, 3);
	Filters::sobel(gausImg2, sIx2, sIy2, sobel2, 3);

	createDisplayImg("Sobel Edges Detection Image 1", sobel1);
	createDisplayImg("Sobel Edges Detection Image 2", sobel2);

	// ----------------- Find corners with Harris -------------------- //
	Mat harris1, harris2;
	HarrisCorner::harris(sIx1, sIy1, harris1, 0.09f, 5);
	HarrisCorner::harris(sIx2, sIy2, harris2, 0.09f, 5);

	//createDisplayImg("Harris Image 1", harris1);
	//createDisplayImg("Harris Image 2", harris2);

	// ----------------- Non Maximum Suppression -------------------- //
	Mat harrisNMS1 = harris1.clone();
	Mat harrisNMS2 = harris2.clone();
	HarrisCorner::nonMaxSuppression(harrisNMS1, 11);
	HarrisCorner::nonMaxSuppression(harrisNMS2, 11);

	createDisplayImg("Harris NonMax Suppression Image 1", harrisNMS1);
	createDisplayImg("Harris NonMax Suppression Image 2", harrisNMS2);

	vector<Point> corners1, corners2;
	HarrisCorner::getCornerPoints(harrisNMS1, corners1, 5.0f);
	HarrisCorner::getCornerPoints(harrisNMS2, corners2, 5.0f);
	HarrisCorner::drawCorners(image1, corners1);
	HarrisCorner::drawCorners(image2, corners2);

	cout << "Corners 1: " << corners1.size() << endl;
	cout << "Corners 2: " << corners2.size() << endl;
	//createDisplayImg("Detected Points Image 1", image1);

	// ----------------- Correspondences of Points -------------------- //

	// SSD Method
	map<Point, Point, PointCompare> correspondences;
	IStrategyCompare *poiCompMethod = new SSD;
	POICorrespondence::findCorrespondences(gausImg1, corners1, gausImg2, corners2, correspondences, 0.3f, 17,
			poiCompMethod);
	delete poiCompMethod;

	Mat bigImg;
	POICorrespondence::plotCorrespondences(image1, corners1, image2, correspondences, bigImg);
	createDisplayImg("Merged Image SSD Comparison", bigImg);

	cout << "SSD Matches Detected: " << correspondences.size() << endl;

	// NCC Method
	correspondences.clear();
	poiCompMethod = new NCC();
	POICorrespondence::findCorrespondences(gausImg1, corners1, gausImg2, corners2, correspondences, 0.989f,
			17, poiCompMethod);
	delete poiCompMethod;

	Mat bigImg2;
	POICorrespondence::plotCorrespondences(image1, corners1, image2, correspondences, bigImg2);
	createDisplayImg("Merged Image NCC Comparison", bigImg2);

	cout << "NCC Matches Detected: " << correspondences.size() << endl;

	waitKey(0);

	image1.release();
	image2.release();
	grayImg1.release();
	grayImg2.release();
	gausImg1.release();
	gausImg2.release();
	sobel1.release();
	sIx1.release();
	sIy1.release();
	sobel2.release();
	sIx2.release();
	sIy2.release();
	harris1.release();
	harrisNMS1.release();
	harris2.release();
	harrisNMS2.release();

	image1.release();
	image2.release();
	bigImg.release();
	bigImg2.release();

	return 0;
}

void openCVtest(Mat &image, Mat &gausImg)
{
	// ------------- Detect Edges with Sobel OpenCV ----------------- //
	Mat sobelCVX, sobelCVY, sobelCV, sobelCVAng;
	Sobel(gausImg, sobelCVX, CV_32F, 1, 0);
	Sobel(gausImg, sobelCVY, CV_32F, 0, 1);
	//sobelCV= abs(sobelCVX)+abs(sobelCVY);
	cartToPolar(sobelCVX, sobelCVY, sobelCV, sobelCVAng);
	sobelCVAng.release();

	createDisplayImg("OpenCV Sobel Edges Detection Image", sobelCV);

	// ----------------- Find corners with Harris OpenCV -------------- //
	Mat harrisCV;
	cornerHarris(gausImg, harrisCV, 5, 7, 0.04f, BORDER_DEFAULT);

	createDisplayImg("Harris Image CV", harrisCV);

	// ----------------- Non Maximum Suppression OpenCV -------------------- //
	Mat harrisCVNMS = harrisCV.clone();
	HarrisCorner::nonMaxSuppression(harrisCVNMS, 11);

	createDisplayImg("Harris NonMax Suppression Image 2", harrisCVNMS);

	Mat image2 = image.clone();
	vector<Point> corners2;
	HarrisCorner::getCornerPoints(harrisCVNMS, corners2, 5.0f);
	HarrisCorner::drawCorners(image2, corners2, 2);

	cout << "Corners: " << corners2.size() << endl;
	//createDisplayImg("Detected Points Image 2", image2);

	sobelCV.release();
	sobelCVX.release();
	sobelCVY.release();
	harrisCV.release();
	harrisCVNMS.release();
}

void correspondencesTest()
{
	/*
	 map<Point,Point,PointCompare> correspondences;
	 IStrategyCompare *poiCompMethod;
	 correspondences.clear();
	 poiCompMethod = new NCC();
	 poiCompMethod->getMeasure(gausImg, Point(5,5), gausImg, Point(5,5), 3);

	 Edges::findCorrespondences(gausImg, corners1, gausImg, corners1, correspondences, 0.2f, 15, poiCompMethod);
	 delete poiCompMethod;

	 Mat bigImg2;
	 Edges::plotCorrespondences(image1, corners1, image2, correspondences, bigImg2);
	 createDisplayImg("Merged Image 2", bigImg2);

	 */
}

void harrisTest()
{
	/*
	 float m[5][5] = {{1,4,3,2,1}, {9,8,5,-1,0}, {4,10,6,1,0},{1,4,2,-1,-2},{0,1,3,-4,-5}};
	 float m2[5][5] = {{7,10,1,3,5}, {1,2,3,4,5}, {5,4,3,2,1},{0,10,20,-5,1},{0,0,2,8,1}};
	 Mat M = Mat(5, 5, CV_32F, m);
	 Mat M2 = Mat(5, 5, CV_32F, m2);
	 Mat dst;

	 cout<<"Original:"<<endl<<M<<endl;
	 cout<<"Original2:"<<endl<<M2<<endl;
	 Edges::harris(M,M2,dst,0.04f,3);
	 cout<<"Harris:"<<endl<<dst<<endl;

	 //Edges::nonMaxSuppression(M,3);
	 //cout<<"Suppressed:"<<endl<<M<<endl;
	 */
}
