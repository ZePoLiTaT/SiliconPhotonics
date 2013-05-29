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

#include "Filters.h"


Filters::Filters()
{
}

Filters::~Filters()
{
}

void Filters::discreteGaussianKernel(float* kernel, float var, int WSize)
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

void Filters::sobelKernel(float* kernelX, float* kernelY, int WSize)
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

float Filters::applyKernel(const Mat& img, int r, int c, float* kernel, int WSize)
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

void Filters::convolution(const Mat& src, Mat &dst, float* kernel, int WSize)
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

void Filters::magnitude(const Mat& mx, const Mat& my, Mat& mm)
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

int Filters::windowSize(float stdev)
{
	return 2 * ceil(3 * stdev) + 1;
}

void Filters::gaussian(const Mat& src, Mat& dst, float stdev)
{
	dst.create(src.rows, src.cols, src.type());

	int WSize = Filters::windowSize(stdev);
	float gausKernel[WSize * WSize];

	cout << ">>>>> W=" << WSize << endl;

	Filters::discreteGaussianKernel(gausKernel, stdev * stdev, WSize);
	Filters::convolution(src, dst, gausKernel, WSize);
}

void Filters::sobel(const Mat& src, Mat& sx, Mat& sy, Mat& s, int WSize)
{
	sx.create(src.rows, src.cols, src.type());
	sy.create(src.rows, src.cols, src.type());
	s.create(src.rows, src.cols, src.type());

	float kernelX[WSize * WSize];
	float kernelY[WSize * WSize];

	Filters::sobelKernel(kernelX, kernelY, WSize);
	//TODO: Remove this line when automatic sobelKernel generation works!
	WSize = 3;

	Filters::convolution(src, sx, kernelX, WSize);
	Filters::convolution(src, sy, kernelY, WSize);

	Filters::magnitude(sx, sy, s);
	//s = abs(sx)+abs(sy);
}

/**
 * The eye responds most strongly to green followed by red and then blue.
 * The NTSC (National Television System Committee) recommends the following
 * formula for color to greyscale conversion:
 *
 * I = .299f * R + .587f * G + .114f * B
 */
void Filters::colorToGray(const Mat& src, Mat& dst)
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


