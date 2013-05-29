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

#include "Edges.h"

using namespace cv;
using namespace std;

inline void printMat(Mat &m)
{
	cout<<"-----------------";
	cout<<m(Rect(0,0,15,15))<<endl;
	cout<<"-----------------F";
}

inline void createDisplayImg(string name, Mat& image)
{
	namedWindow(name, CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow(name, image);                   // Show our image inside it.
}

int main( int argc, char** argv )
{
	string fname;
	if( argc != 2)
	{
		cout <<" Usage: main Image" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image.data)                                // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	createDisplayImg("Original Image", image);

	// ----------------- Convert Gray Scale -------------------- //
	Mat grayImg;
	//cvtColor(image, grayImg, CV_BGR2GRAY);
	Edges::colorToGray(image,grayImg);
	createDisplayImg("Gray Image", grayImg);

	// ----------------- Smooth with Gaussian Kernel -------------------- //

	float stdev=1;
	Mat gausImg;
	Edges::gaussian(grayImg, gausImg, stdev);

	createDisplayImg("Gaussian Blur Image", gausImg);

	// ----------------- Detect Edges with Sobel -------------------- //
	Mat sIx, sIy, sobel;
	Edges::sobel(gausImg, sIx, sIy, sobel, 3);

	createDisplayImg("Sobel Edges Detection Image", sobel);

	// ------------- Detect Edges with Sobel OpenCV ----------------- //
	Mat sobelCVX, sobelCVY, sobelCV, sobelCVAng;
	Sobel(gausImg, sobelCVX, CV_32F, 1, 0);
	Sobel(gausImg, sobelCVY, CV_32F, 0, 1);
	//sobelCV= abs(sobelCVX)+abs(sobelCVY);
	cartToPolar(sobelCVX,sobelCVY,sobelCV,sobelCVAng);
	sobelCVAng.release();

	createDisplayImg("OpenCV Sobel Edges Detection Image", sobelCV);

	// ----------------- Find corners with Harris -------------------- //
	Mat harris;
	Edges::harris(sIx,sIy,harris,0.09f,5);

	createDisplayImg("Harris Image", harris);

	// ----------------- Non Maximum Suppression -------------------- //
	Mat harrisNMS = harris.clone();
	Edges::nonMaxSuppression(harrisNMS,11);

	createDisplayImg("Harris NonMax Suppression Image 1", harrisNMS);

	Mat image1 = image.clone();
	vector<Point> corners1;
	Edges::getCornerPoints(harrisNMS, corners1, 5.0f);
	Edges::drawCorners(image1, corners1, 2);

	cout<<"Corners: "<<corners1.size()<<endl;
	//createDisplayImg("Detected Points Image 1", image1);


	// ----------------- Find corners with Harris OpenCV -------------- //
	Mat harrisCV;
	cornerHarris(gausImg,harrisCV,5,7,0.04f,BORDER_DEFAULT);

	createDisplayImg("Harris Image CV", harrisCV);

	// ----------------- Non Maximum Suppression OpenCV -------------------- //
	Mat harrisCVNMS = harrisCV.clone();
	Edges::nonMaxSuppression(harrisCVNMS,11);

	createDisplayImg("Harris NonMax Suppression Image 2", harrisCVNMS);

	Mat image2 = image.clone();
	vector<Point> corners2;
	Edges::getCornerPoints(harrisCVNMS, corners2, 5.0f);
	Edges::drawCorners(image2, corners2, 2);

	cout<<"Corners: "<<corners2.size()<<endl;
	//createDisplayImg("Detected Points Image 2", image2);

	// ----------------- Correspondences of Points -------------------- //

	// SSD Method
	map<Point,Point,PointCompare> correspondences;
	IStrategyCompare *poiCompMethod = new SSD;
	Edges::findCorrespondences(gausImg, corners1, gausImg, corners2, correspondences, 0.0f, 15, poiCompMethod);
	delete poiCompMethod;

	Mat bigImg;
	Edges::plotCorrespondences(image1, corners1, image2, correspondences, bigImg);
	createDisplayImg("Merged Image", bigImg);

	// NCC Method
	correspondences.clear();
	poiCompMethod = new NCC();
	Edges::findCorrespondences(gausImg, corners1, gausImg, corners2, correspondences, 0.99f, 15, poiCompMethod);
	delete poiCompMethod;

	Mat bigImg2;
	Edges::plotCorrespondences(image1, corners1, image2, correspondences, bigImg2);
	createDisplayImg("Merged Image 2", bigImg2);

	waitKey(0);

	image.release();
	grayImg.release();
	gausImg.release();
	sobel.release();
	sIx.release();
	sIy.release();
	harris.release();
	harrisNMS.release();

	image1.release();
	image2.release();
	bigImg.release();
	bigImg2.release();

	sobelCV.release();
	sobelCVX.release();
	sobelCVY.release();
	harrisCV.release();
	harrisCVNMS.release();

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

	return 0;
}


