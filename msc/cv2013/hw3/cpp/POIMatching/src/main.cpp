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
	Mat sobelX, sobelY, sobel;
	Edges::sobel(gausImg, sobelX, sobelY, sobel, 3);

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
	Edges::harris(sobelX,sobelY,harris,0.04,7);

	createDisplayImg("Harris Image", harris);

	// ----------------- Find corners with Harris OpenCV -------------- //
	Mat harrisCV;
	cornerHarris(gausImg,harrisCV,5,7,0.04,BORDER_DEFAULT);

	createDisplayImg("Harris Image CV", harrisCV);

	// ----------------- Non Maximum Suppression -------------------- //
	Mat harrisNMS = harris.clone();
	Edges::nonMaxSuppression(harrisNMS,20);

	createDisplayImg("Harris NonMax Suppression Image", harrisNMS);

	vector<Point> corners;
	Edges::getCornerPoints(harrisNMS, corners, 4.5);
	Edges::drawCorners(image, corners, 2);

	cout<<"Corners: "<<corners.size()<<endl;
	createDisplayImg("Detected Points Image", image);

	waitKey(0);

	image.release();
	grayImg.release();
	gausImg.release();
	sobel.release();
	sobelX.release();
	sobelY.release();
	harris.release();
	harrisNMS.release();

	sobelCV.release();
	sobelCVX.release();
	sobelCVY.release();
	harrisCV.release();


	return 0;
}


