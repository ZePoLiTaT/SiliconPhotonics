/* =====================================================================================
 *
 *       Filename:  Edges.cpp
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

#include "Edges.h"

inline int getIdx(int val, int MAX)
{
	if (val < 0)
		return 0;
	if (val >= MAX)
		return MAX - 0.01;
	return val;
}

Edges::Edges()
{
}

Edges::~Edges()
{
}

void Edges::discreteGaussianKernel(float* kernel, float var, int WSize)
{
	int WHalf = floor(WSize / 2);
	float x, y;
	float sum = 0;

	//Calculate discrete coefficients
	for (int i = 0; i < WSize; ++i)
	{
		for (int j = 0; j < WSize; ++j)
		{
			x = i - WHalf;
			y = j - WHalf;
			kernel[i * WSize + j] = 1.0 / (2 * var * M_PI) * exp(-(x * x + y * y) / (2 * var));
			sum += kernel[i * WSize + j];
			//printf("[%f] ", kernel[i * WSize + j]);
		}
		//printf("\n");
	}
	//printf("SUMA=%f\n", sum);

	//Normalize
	for (int i = 0; i < WSize; ++i)
	{
		for (int j = 0; j < WSize; ++j)
		{
			kernel[i * WSize + j] /= sum;
			printf("[%f] ", kernel[i * WSize + j]);
		}
		printf("\n");
	}
}

void Edges::sobelKernel(float* kernelX, float* kernelY, int WSize)
{
	//TODO: Implement pascal triangle to get coefficients from WSize
	/* kernelX = (float *)malloc(WSize*WSize*sizeof(float));
	 kernelY = (float *)malloc(WSize*WSize*sizeof(float));
	 calcPascalCoefs(baseKernX, WSize);
	 for (int i = 0; i < WSize; ++i)
	 {
	 for (int j = 0; j < WSize; ++j)
	 {
	 baseKernY[i*WSize+j] = baseKernX[j*WSize+i];
	 }
	 }*/

	float baseKernX[] =
	{ 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	float baseKernY[] =
	{ 1, 2, 1, 0, 0, 0, -1, -2, -1 };

	for (int i = 0; i < WSize; ++i)
	{
		for (int j = 0; j < WSize; ++j)
		{
			kernelX[i * WSize + j] = baseKernX[i * WSize + j];
			kernelY[i * WSize + j] = baseKernY[i * WSize + j];
		}
	}
}

float Edges::applyKernel(const Mat& img, int r, int c, float* kernel, int WSize)
{
	int curR, curC;
	int WHalf = floor(WSize / 2);
	float sum = 0;

	for (int i = 0; i < WSize; ++i)
	{
		curR = getIdx(r + (i - WHalf), img.rows);
		const mtype* imgP = img.ptr<mtype>(curR);

		for (int j = 0; j < WSize; ++j)
		{
			curC = getIdx(c + (j - WHalf), img.cols);
			//printf("-->[%d,%d]{%d}",curR,curC,iPtrRow[curC]);
			sum += imgP[curC] * kernel[i * WSize + j];
		}
		//printf("\n");
	}
	//printf("sum[%d][%d]:{%f}\n\n",r,c,sum);

	return sum;
}

void Edges::convolution(const Mat& src, Mat &dst, float* kernel, int WSize)
{
	dst.create(src.rows, src.cols, src.type());

	for (int i = 0; i < src.rows; ++i)
	{
		mtype* dstP = dst.ptr<mtype>(i);
		for (int j = 0; j < src.cols; ++j)
		{
			float sum = applyKernel(src, i, j, kernel, WSize);
			dstP[j] = sum;
		}
		//printf("\n");
	}
}

void Edges::magnitude(const Mat& mx, const Mat& my, Mat& mm)
{
	mm.create(mx.rows, mx.cols, mx.type());

	// number of lines
	int nr = mx.rows;
	int nc = mx.cols * mx.channels();
	if (mx.isContinuous())
	{
		// since its unwrapped, it can be seen as only 1 row and WxH cols
		nc = nc * nr;
		nr = 1;
	}

	float norm;
	for (int i = 0; i < nr; i++)
	{
		const mtype* mxPtr = mx.ptr<mtype>(i);
		const mtype* myPtr = my.ptr<mtype>(i);
		mtype* mmPtr = mm.ptr<mtype>(i);
		for (int j = 0; j < nc; j++)
		{
			norm = (float) mxPtr[j] * mxPtr[j] + (float) myPtr[j] * myPtr[j];
			mmPtr[j] = sqrt(norm);
		}
	}
}

int Edges::windowSize(float stdev)
{
	return 2 * ceil(3 * stdev) + 1;
}

void Edges::gaussian(const Mat& src, Mat& dst, float stdev)
{
	dst.create(src.rows, src.cols, src.type());

	int WSize = Edges::windowSize(stdev);
	float gausKernel[WSize * WSize];

	cout << ">>>>> W=" << WSize << endl;

	Edges::discreteGaussianKernel(gausKernel, stdev * stdev, WSize);
	Edges::convolution(src, dst, gausKernel, WSize);
}

void Edges::sobel(const Mat& src, Mat& sx, Mat& sy, Mat& s, int WSize)
{
	sx.create(src.rows, src.cols, src.type());
	sy.create(src.rows, src.cols, src.type());
	s.create(src.rows, src.cols, src.type());

	float kernelX[WSize * WSize];
	float kernelY[WSize * WSize];

	Edges::sobelKernel(kernelX, kernelY, WSize);
	//TODO: Remove this line when automatic sobelKernel generation works!
	WSize = 3;

	Edges::convolution(src, sx, kernelX, WSize);
	Edges::convolution(src, sy, kernelY, WSize);

	Edges::magnitude(sx, sy, s);
	//s = abs(sx)+abs(sy);
}

/**
 * The eye responds most strongly to green followed by red and then blue.
 * The NTSC (National Television System Committee) recommends the following
 * formula for color to greyscale conversion:
 *
 * I = .299f * R + .587f * G + .114f * B
 */
void Edges::colorToGray(const Mat& src, Mat& dst)
{
	dst.create(src.rows, src.cols, CV_32F);

	for (int i = 0; i < src.rows; ++i)
	{
		const Vec3b* srcP = src.ptr<Vec3b>(i);
		mtype* dstP = dst.ptr<mtype>(i);

		for (int j = 0; j < src.cols; ++j)
		{
			dstP[j] = (.114f * srcP[j][0] + .587f * srcP[j][1] + .299f * srcP[j][2]) / 256.0f;
		}
	}
}

/**
 * Edges detection based on differential on both dimensions
 * and a threshold k
 */
void Edges::harris(const Mat& sIx, const Mat& sIy, Mat& dst, float k, int WSize)
{
	dst.create(sIx.rows, sIx.cols, sIx.type());

	float A, B, C, trApB;
	int WHalf = floor(WSize / 2);
	int twi, twj;

	for (int i = 0; i < sIx.rows; ++i)
	{
		mtype* dstP = dst.ptr<mtype>(i);
		for (int j = 0; j < sIx.cols; ++j)
		{
			A = 0, B = 0, C = 0;
			for (int wi = -WHalf; wi <= WHalf; ++wi)
			{
				twi = getIdx(i + wi, sIx.rows);
				const mtype* sIxP = sIx.ptr<mtype>(twi);
				const mtype* sIyP = sIy.ptr<mtype>(twi);

				for (int wj = -WHalf; wj <= WHalf; ++wj)
				{
					twj = getIdx(j + wj, sIx.cols);

					A += sIxP[twj] * sIxP[twj];
					B += sIyP[twj] * sIyP[twj];
					C += sIxP[twj] * sIyP[twj];
				}
			}

			trApB = A + B;
			dstP[j] = A * B - C * C - k * (trApB * trApB);
		}
	}
}

/**
 * Non Maximum suppression
 */
void Edges::nonMaxSuppression(Mat& mat, int WSize)
{
	if (mat.empty())
		return;

	int WHalf = floor(WSize / 2);
	int twi, twj;
	//bool suppressed;

	for (int i = 0; i < mat.rows; ++i)
	{
		mtype* matP = mat.ptr<mtype>(i);
		for (int j = 0; j < mat.cols; ++j)
		{
			//suppressed = false;
			for (int wi = -WHalf; wi <= WHalf /*&& !suppressed*/; ++wi)
			{
				twi = getIdx(i + wi, mat.rows);
				mtype* wmatP = mat.ptr<mtype>(twi);

				for (int wj = -WHalf; wj <= WHalf /*&& !suppressed*/; ++wj)
				{
					twj = getIdx(j + wj, mat.cols);

					if (matP[j] < wmatP[twj])
					{
						matP[j] = 0;
						//suppressed = true;
					}
				}
			}
		}
	}
}

/*
 * Obtain the corners that are greater than a threshold as (x,y) points
 */
void Edges::getCornerPoints(Mat& mat, vector<Point>& corners, mtype threshold)
{
	for (int i = 0; i < mat.rows; ++i)
	{
		mtype* matP = mat.ptr<mtype>(i);
		for (int j = 0; j < mat.cols; ++j)
		{
			if (matP[j] > threshold)
				corners.push_back(Point(j, i));

		}
	}
}

/*
 * Draws the corners as circles over the image
 */
void Edges::drawCorners(Mat& img, vector<Point>& corners, int radius)
{
	Scalar color(0, 255, 255);

	vector<Point>::iterator it;
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		circle(img, *it, radius, color, 2);
	}
}

void Edges::findCorrespondences(const Mat &img1, vector<Point>& corners1, const Mat &img2,
		vector<Point>& corners2, map<Point, Point, PointCompare> &correspondences, float threshold,
		int WSize, IStrategyCompare *poiCompareMethod)
{
	mtype val, topval;
	Point minpoint;
	bool firstComparison;

	vector<Point>::iterator it1, it2;
	for (it1 = corners1.begin(); it1 != corners1.end(); ++it1)
	{
		firstComparison = true;
		for (it2 = corners2.begin(); it2 != corners2.end(); ++it2)
		{
			val = poiCompareMethod->getMeasure(img1, *it1, img2, *it2, WSize);
			if(firstComparison)
			{
				minpoint = *it2;
				topval = val;
				firstComparison = false;
			}
			else if (poiCompareMethod->compare(val, topval))
			{
				minpoint = *it2;
				topval = val;
			}
		}

		cout << topval << endl;
		if (poiCompareMethod->compare(topval, threshold))
			correspondences[*it1] = minpoint;
	}
}

void Edges::plotCorrespondences(const Mat& img1, vector<Point>& corners1, const Mat& img2,
		map<Point, Point, PointCompare> &correspondences, Mat& dst)
{
	dst.create(max(img1.rows, img2.rows), img1.cols + img2.cols, img1.type());

	//ROI of bigger image pointing to first half
	Mat dstImg1 = dst(Rect(0, 0, img1.cols, img1.rows));
	//ROI of bigger image pointing to second half
	Mat dstImg2 = dst(Rect(img1.cols, 0, img2.cols, img2.rows));

	img1.copyTo(dstImg1);
	img2.copyTo(dstImg2);

	//Draw green lines
	Scalar colorL(0, 255, 0);

	int xP2;
	map<Point, Point>::iterator it;
	for (it = correspondences.begin(); it != correspondences.end(); ++it)
	{
		//Displace point in image2 since its located in the second half of the big image
		xP2 = it->second.x + img1.cols;
		line(dst, it->first, Point(xP2, it->second.y), colorL, 1, CV_AA);
	}
}
