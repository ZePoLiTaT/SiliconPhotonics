/* =====================================================================================
 *
 *       Filename:  poicomparemethods.h
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

#ifndef POICOMPAREMETHODS_H_
#define POICOMPAREMETHODS_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "Utils.h"

using namespace cv;
using namespace std;

typedef float mtype;

namespace poicomparemethods
{

/****************** class prototypes ****************/

//Interface of Strategy pattern for each comparison algorithm
class IStrategyCompare;
//Implementation of Normalized Cross Correlation comparison algorithm under Strategy pattern
class NCC;
//Implementation of Sum of Squared Differences comparison algorithm under Strategy pattern
class SSD;


/****************** Description of each class ****************/
class IStrategyCompare
{
public:
	virtual ~IStrategyCompare()	{}
	virtual mtype getMeasure(const Mat &f1, Point p1, const Mat& f2, Point p2,
			int WSize) = 0;
	virtual bool compare(mtype value, mtype threshold) = 0;
};

class SSD: public IStrategyCompare
{
public:
	SSD();
	virtual mtype getMeasure(const Mat &f1, Point p1, const Mat& f2, Point p2, int WSize);
	virtual bool compare(mtype value, mtype threshold);
};

class NCC: public IStrategyCompare
{
public:
	NCC();
	NCC(mtype f1m, mtype f2m);
	virtual mtype getMeasure(const Mat &f1, Point p1, const Mat& f2, Point p2, int WSize);
	virtual bool compare(mtype value, mtype threshold);
};

}
#endif /* POICOMPAREMETHODS_H_ */
