/* =====================================================================================
 *
 *       Filename:  poicomparemethods.cpp
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

#include "poicomparemethods.h"

namespace poicomparemethods
{

inline int getIdx(int val, int MAX)
{
	if (val < 0)
		return 0;
	if (val >= MAX)
		return MAX - 0.01;
	return val;
}

SSD::SSD()
{
}

mtype SSD::getMeasure(const Mat &f1, Point p1, const Mat& f2, Point p2, int WSize)
{
	int WHalf = floor(WSize / 2);
	int f1i, f1j, f2i, f2j;

	mtype sum = 0, diff;

	for (int wi = -WHalf; wi <= WHalf; ++wi)
	{
		f1i = getIdx(p1.y + wi, f1.rows);
		f2i = getIdx(p2.y + wi, f2.rows);

		const mtype* f1P = f1.ptr<mtype>(f1i);
		const mtype* f2P = f2.ptr<mtype>(f2i);

		for (int wj = -WHalf; wj <= WHalf; ++wj)
		{
			f1j = getIdx(p1.x + wj, f1.cols);
			f2j = getIdx(p2.x + wj, f2.cols);

			diff = f1P[f1j] - f2P[f2j];
			sum += diff * diff;
		}
	}

	return sum;
}

bool SSD::compare(mtype value, mtype threshold)
{
	return value <= threshold;
}

NCC::NCC()
{
	this->f1mean = 0.0f;
	this->f2mean = 0.0f;
}

NCC::NCC(mtype f1m, mtype f2m)
{
	this->f1mean = f1m;
	this->f2mean = f2m;
}

mtype NCC::getMeasure(const Mat& f1, Point p1, const Mat& f2, Point p2, int WSize)
{
	int WHalf = floor(WSize / 2);
	int f1i, f1j, f2i, f2j;

	mtype sumf1 = 0, sumf2 = 0, sumf1_2 = 0, sumf2_2 = 0, sumf1f2 = 0;
	mtype upncc, downncc, ncc;

	for (int wi = -WHalf; wi <= WHalf; ++wi)
	{
		f1i = getIdx(p1.y + wi, f1.rows);
		f2i = getIdx(p2.y + wi, f2.rows);

		const mtype* f1P = f1.ptr<mtype>(f1i);
		const mtype* f2P = f2.ptr<mtype>(f2i);

		for (int wj = -WHalf; wj <= WHalf; ++wj)
		{
			f1j = getIdx(p1.x + wj, f1.cols);
			f2j = getIdx(p2.x + wj, f2.cols);

			sumf1 += f1P[f1j];
			sumf2 += f2P[f2j];
			sumf1_2 += f1P[f1j] * f1P[f1j];
			sumf2_2 += f2P[f2j] * f2P[f2j];
			sumf1f2 += f1P[f1j] * f2P[f2j];
		}
	}

	sumf1 /= WSize;
	sumf2 /= WSize;

	upncc = sumf1f2 - sumf1 * sumf2;
	downncc = (sumf1_2 - sumf1 * sumf1) * (sumf2_2 - sumf2 * sumf2);

	ncc = upncc / sqrt(downncc);
	return ncc;
}

bool NCC::compare(mtype value, mtype threshold)
{
	return abs(value) >= threshold;
}

}
