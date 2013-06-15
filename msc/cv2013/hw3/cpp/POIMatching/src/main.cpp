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

#include <fstream>

#include "libs/Filters.h"
#include "libs/HarrisCorner.h"
#include "libs/POICorrespondence.h"

using namespace cv;
using namespace std;
using namespace features;
using namespace poi;


/*void harrisTest();
void sobelTest();
void harrisTest2();*/

int process(string i1name, string i2name, float stdev, float harrisK, int harrisW,
		float nmsThrs, float ssdThrs, float nccThrs, int corrW, int corrKnownMovement,
		multimap<Point, Point, PointCompare> &nccMatches)
{
	printf("\\hline\n");

	// -------------------------------------------------------------- //
	// ------------------------ Load Images ------------------------- //
	// -------------------------------------------------------------- //
	Mat image, image1, image2;
	image1 = imread(i1name, CV_LOAD_IMAGE_COLOR);   // Read the file
	image2 = imread(i2name, CV_LOAD_IMAGE_COLOR);   // Read the file

	if (!image1.data || !image2.data)                 // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	createDisplayImg("Original Image", image1, image2, image);


	// -------------------------------------------------------------- //
	// ----------------- Convert Gray Scale ------------------------- //
	// -------------------------------------------------------------- //
	Mat grayImg, grayImg1, grayImg2;
	Filters::colorToGray(image1, grayImg1);
	Filters::colorToGray(image2, grayImg2);

	// -------------------------------------------------------------- //
	// ----------------- Smooth with Gaussian Kernel ---------------- //
	// -------------------------------------------------------------- //

	Mat gausImg, gausImg1, gausImg2;
	Filters::gaussian(grayImg1, gausImg1, stdev);
	Filters::gaussian(grayImg2, gausImg2, stdev);
	//createDisplayImg("Gaussian Blur", gausImg1, gausImg2, gausImg);

	// -------------------------------------------------------------- //
	// ----------------- Detect Edges with Sobel -------------------- //
	// -------------------------------------------------------------- //
	Mat sIx1, sIy1, sobel1, sobel;
	Mat sIx2, sIy2, sobel2;
	Filters::sobel(gausImg1, sIx1, sIy1, sobel1);
	Filters::sobel(gausImg2, sIx2, sIy2, sobel2);
	createDisplayImg("Sobel Edges Detection", sobel1, sobel2, sobel);

	// -------------------------------------------------------------- //
	// ----------------- Find corners with Harris ------------------- //
	// -------------------------------------------------------------- //
	Mat harris, harris1, harris2;
	HarrisCorner::harris(sIx1, sIy1, harris1, harrisK, harrisW);
	HarrisCorner::harris(sIx2, sIy2, harris2, harrisK, harrisW);
	createDisplayImg("Harris", harris1, harris2, harris);

	vector<Point> cornersH1, cornersH2;
	HarrisCorner::getCornerPoints(harris1, cornersH1, nmsThrs);
	HarrisCorner::getCornerPoints(harris2, cornersH2, nmsThrs);
	printf("Harris Corners L & %d \\\\\n", (int)cornersH1.size());
	printf("Harris Corners R & %d \\\\\n", (int)cornersH2.size());

	// -------------------------------------------------------------- //
	// ----------------- Non Maximum Suppression -------------------- //
	// -------------------------------------------------------------- //
	Mat harrisNMS, harrisNMS1, harrisNMS2;
	HarrisCorner::nonMaxSuppression(harris1, harrisNMS1, 11);
	HarrisCorner::nonMaxSuppression(harris2, harrisNMS2, 11);
	//createDisplayImg("Harris NonMax Suppression", harrisNMS1, harrisNMS2, harrisNMS);

	vector<Point> cornersNMS1, cornersNMS2;
	HarrisCorner::getCornerPoints(harrisNMS1, cornersNMS1, nmsThrs);
	HarrisCorner::getCornerPoints(harrisNMS2, cornersNMS2, nmsThrs);
	HarrisCorner::drawCorners(image1, cornersNMS1);
	HarrisCorner::drawCorners(image2, cornersNMS2);

	printf("NMS Corners L & %d \\\\\n", (int)cornersNMS1.size());
	printf("NMS Corners R & %d \\\\\n", (int)cornersNMS2.size());

	// -------------------------------------------------------------- //
	// ----------------- Correspondences of Points ------------------ //
	// -------------------------------------------------------------- //
	// -----------------------> SSD Method
	// -------------------------------------------------------------- //
	multimap<Point, Point, PointCompare> ssdMatches;
	IStrategyCompare *poiCompMethod = new SSD;
	POICorrespondence::findCorrespondences(gausImg1, cornersNMS1, gausImg2, cornersNMS2,
			ssdMatches, ssdThrs, corrW, corrKnownMovement, poiCompMethod);
	delete poiCompMethod;

	Mat ssd;
	POICorrespondence::plotCorrespondences(image1, cornersNMS1, image2, ssdMatches,
			ssd);
	createDisplayImg("Merged Image SSD Comparison", ssd);


	printf("SSD Matches & %d \\\\\n", (int)ssdMatches.size());

	// -------------------------------------------------------------- //
	// -----------------------> NCC Method
	// -------------------------------------------------------------- //
	//map<Point, Point, PointCompare> nccMatches;
	poiCompMethod = new NCC();
	POICorrespondence::findCorrespondences(gausImg1, cornersNMS1, gausImg2, cornersNMS2,
			nccMatches, nccThrs, corrW, corrKnownMovement, poiCompMethod);
	delete poiCompMethod;

	Mat ncc;
	POICorrespondence::plotCorrespondences(image1, cornersNMS1, image2, nccMatches,
			ncc);
	createDisplayImg("Merged Image NCC Comparison", ncc);

	printf("NCC Matches & %d \\\\\n", (int)nccMatches.size());

	//imwrite( "../../doc/figs/img2ssd.png", ssd );
	//imwrite( "../../doc/figs/img2ncc.png", ncc );
	//printf("& %.2f & %d & %.2f & %d \\\\", ssdThrs, ssdMatches.size(), nccThrs, nccMatches.size() );
	//printf("%.2f & %d & %d\\\\\n", nmsThrs, cornersNMS1.size(), cornersNMS2.size());
	//printf("%.2f & %d\\\\\n", nccThrs, nccMatches.size());

	return 0;
}

void correspondencesToFile(const char* fname, multimap<Point, Point, PointCompare> &matches)
{
	ofstream myfile;
	myfile.open (fname, ios::out | ios::trunc);

	myfile<<matches.size()<<endl;

	multimap<Point,Point>::iterator it;
	for(it = matches.begin(); it != matches.end(); ++it)
	{
		myfile<<it->first.x<<" "<<it->first.y<<" "<<it->second.x<<" "<<it->second.y<<endl;
	}
}

int main(int argc, char** argv)
{
	int result = 0;
	string fnameI1, fnameI2;
	multimap<Point, Point, PointCompare> nccMatches;

	float stdev = 0.6f;
	float harrisK = 0.099f;
	int harrisW = 5;
	float harrisThrs = 0.4f;
	float ssdThrs = 0.4f;
	float nccThrs = 0.97f;
	int corrW = 11;
	int corrKnownMovement = 200;

	if (argc != 7)
	{
		cout << " Usage: main Image1 Image2 harrisK harrisThrs ssdThrs nccThrs" << endl;
		cout << " Setting default to: main img/benevolentAnnika.jpg img/benevolentAnnikaRot.jpg 0.09f 0.4f 0.4f 0.97f" << endl;

		fnameI1 = "img/benevolentAnnika.jpg";
		fnameI2 = "img/benevolentAnnikaRot.jpg";
		harrisK = 0.09f;
		harrisThrs = 0.4f;
		ssdThrs = 0.4f;
		nccThrs = 0.96f;
	}
	else
	{
		fnameI1 = argv[1];
		fnameI2 = argv[2];
		harrisK = atof(argv[3]);
		harrisThrs = atof(argv[4]);
		ssdThrs = atof(argv[5]);
		nccThrs = atof(argv[6]);
	}

	//Parameters benevolent.png
	/*fnameI1 = "img/benevolentAnnika.jpg";
	fnameI2 = "img/benevolentAnnikaRot.jpg";
	float stdev = 0.6f;
	float harrisK = 0.09f;
	int harrisW = 5;
	float harrisThrs = 0.4f;
	float ssdThrs = 0.4f;
	float nccThrs = 0.97f;
	int corrW = 11;
	int corrKnownMovement = 100;*/

	//Parameters Test SSD vs NCC high light change benevolent.png
	/*fnameI1 = "img/benevolentAnnika.jpg";
	fnameI2 = "img/benevolentAnnikaRotLight.jpg";
	ssdThrs = 0.8f;
	nccThrs = 0.92f;*/

	//Parameters puregeometry.jpg
	/*fnameI1 = "img/puregeometryRomanowsky.png";
	fnameI2 = "img/puregeometryRomanowskyPersp.png";
	harrisK = 0.04f;
	harrisThrs = 0.25f;
	ssdThrs = 1.2f;
	nccThrs = 0.92f;
	corrKnownMovement = 180;*/

	//Parameters foto.jpg
	/*fnameI1 = "img/foto1.jpg";
	fnameI2 = "img/foto2.jpg";
	harrisK = 0.09f;
	harrisThrs = 0.45f;
	ssdThrs = 0.8f;
	nccThrs = 0.95f;
	corrKnownMovement = 180;*/

	printf("Stdev & %.2f \\\\\n", stdev);
	printf("Harris k & %.2f \\\\\n", harrisK);
	printf("Harris WSize & %d \\\\\n", harrisW);
	printf("NMS Thold & %.2f \\\\\n", harrisThrs);
	printf("SSD Thold & %.2f \\\\\n", ssdThrs);
	printf("NCC Thold & %.2f \\\\\n", nccThrs);
	printf("Correspondences WSize & %d \\\\\n", corrW);
	printf("Correspondences knownDisplace & %d \\\\\n", corrKnownMovement);

	result = process(fnameI1, fnameI2, stdev, harrisK, harrisW, harrisThrs, ssdThrs, nccThrs, corrW, corrKnownMovement, nccMatches);
	correspondencesToFile("correspondences.txt", nccMatches);
	//harrisTest2();

	waitKey(0);
	return result;
}

void openCVtest(Mat &image, Mat &gausImg)
{
	/*
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
	*/
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

	 float m[5][5] = {{1,4,3,2,1}, {9,8,5,-1,0}, {4,10,6,1,0},{1,4,2,-1,-2},{0,1,3,-4,-5}};
	 float m2[5][5] = {{7,10,1,3,5}, {1,2,3,4,5}, {5,4,3,2,1},{0,10,20,-5,1},{0,0,2,8,1}};
	 Mat M = Mat(5, 5, CV_32F, m);
	 Mat M2 = Mat(5, 5, CV_32F, m2);
	 Mat dst;

	 cout<<"Original:"<<endl<<M<<endl;
	 cout<<"Original2:"<<endl<<M2<<endl;
	 HarrisCorner::harris(M,M2,dst,0.04f,3);
	 cout<<"Harris:"<<endl<<dst<<endl;
}

void sobelTest()
{

	 /*float m[5][5] = {{1,4,3,253,250}, {9,8,5,-1,0}, {4,10,6,1,0},{1,4,2,-1,-2},{0,1,3,-4,-5}};

	 Mat M = Mat(5, 5, CV_32F, m);
	 Mat Sx, Sy, s;

	 cout<<"Original:"<<endl<<M<<endl;
	 Filters::sobel(M, Sx, Sy, s);
	 cout<<"Sobel X:"<<endl<<Sx<<endl;
	 cout<<"Sobel Y:"<<endl<<Sy<<endl;
	 cout<<"Sobel:"<<endl<<s<<endl;*/
}

void harrisTest2()
{
	/*Mat M, Mg2, Mg;
	Mat Sx, Sy, s, h, hnms;

	string fnameI1 = "img/casa.jpeg";
	M = imread(fnameI1, CV_LOAD_IMAGE_COLOR);   // Read the file

	Filters::colorToGray(M, Mg2);
	Filters::gaussian(Mg2, Mg, 0.6f);
	Filters::sobel(Mg, Sx, Sy, s);
	HarrisCorner::harris(Sx, Sy, h, 0.08, 7);
	HarrisCorner::nonMaxSuppression(h, hnms, 10);
	vector<Point> corners;
	HarrisCorner::getCornerPoints(hnms, corners, 0.4f);
	HarrisCorner::drawCorners(M, corners);

	createDisplayImg("Original", Mg);
	createDisplayImg("Sobel", s);
	createDisplayImg("Harris", h);
	createDisplayImg("Harris NMS", hnms);
	createDisplayImg("Points", M);


	//cout << "Original:" << endl << Mg << endl;
	//cout << "Gray X:" << endl << Mg << endl;
	cout << "Sobel X:" << endl << Sx(Rect(18,0,3,3))<<endl;
	cout << "Sobel Y:" << endl << Sy(Rect(18,0,3,3))<<endl;
	cout << "Harris:" << endl << h(Rect(18,0,3,3))<<endl;
	//cout << "Sobel Y:" << endl << Sy << endl;
	//cout << "Sobel:" << endl << s << endl;*/
}
