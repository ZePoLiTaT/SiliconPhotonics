/* =====================================================================================
 *
 *       Filename:  Utils.h
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


#ifndef UTILS_H_
#define UTILS_H_

typedef float mtype;

using namespace cv;
using namespace std;

inline int getIdx(int val, int MAX)
{
	if (val < 0)
		return 0;
	if (val >= MAX)
		return MAX - 0.01;
	return val;
}

inline void printMat(Mat &m)
{
	//cout<<"-----------------";
	//cout<<m(Rect(0,0,15,15))<<endl;
	//cout<<"-----------------F";
}

inline void createDisplayImg(string name, Mat& image)
{
	namedWindow(name, CV_WINDOW_AUTOSIZE); // Create a window for display.
	imshow(name, image);                   // Show our image inside it.
}

inline void createDisplayImg(string name, Mat& image1, Mat& image2, Mat &dst)
{
	dst.create(max(image1.rows, image2.rows), image1.cols + image2.cols, image1.type());

	//ROI of bigger image pointing to first half
	Mat dstImg1 = dst(Rect(0, 0, image1.cols, image1.rows));
	//ROI of bigger image pointing to second half
	Mat dstImg2 = dst(Rect(image1.cols, 0, image2.cols, image2.rows));

	image1.copyTo(dstImg1);
	image2.copyTo(dstImg2);
}

#endif /* UTILS_H_ */
