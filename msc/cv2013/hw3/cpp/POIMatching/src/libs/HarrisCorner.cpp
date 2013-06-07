/* =====================================================================================
 *
 *       Filename:  Harris.cpp
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

#include "HarrisCorner.h"

namespace features
{

HarrisCorner::HarrisCorner()
{
	//calibrate(5,11,0.09f);
}
/*
void Harris::calibrate(int WHarrisSize, int WNMSSize, float k)
{
	this->WHarrisSize = WHarrisSize;
	this->WNMSSize = WNMSSize;
	this->k = k;
}*/

/**
 * Edges detection based on differential on both dimensions
 * and a threshold k
 */
void HarrisCorner::harris(const Mat& sIx, const Mat& sIy, Mat& dst, float k, int WSize)
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

	Filters::normalize(dst,dst);
}

/**
 * Non Maximum suppression
 */
void HarrisCorner::nonMaxSuppression(Mat& mat, Mat &dst, int WSize)
{
	dst = mat.clone();
	if (mat.empty())
		return;

	int WHalf = floor(WSize / 2);
	int twi, twj;
	//bool suppressed;

	for (int i = 0; i < mat.rows; ++i)
	{
		mtype* dstP = dst.ptr<mtype>(i);
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

					if (dstP[j] < wmatP[twj])
					{
						dstP[j] = 0;
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
void HarrisCorner::getCornerPoints(Mat& mat, vector<Point>& corners, mtype threshold)
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
void HarrisCorner::drawCorners(Mat& img, vector<Point>& corners, int radius)
{
	Scalar color(0, 255, 255);

	vector<Point>::iterator it;
	for (it = corners.begin(); it != corners.end(); ++it)
	{
		circle(img, *it, radius, color, 2);
	}
}

} /* namespace poisim */
