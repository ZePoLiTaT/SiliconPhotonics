/* =====================================================================================
 *
 *       Filename:  POICorrespondence.h
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

#ifndef POICORRESPONDENCE_H_
#define POICORRESPONDENCE_H_

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "POICompareMethods.h"

using namespace cv;
using namespace std;

namespace poi
{

struct PointCompare
{
	bool operator()(const Point& lhs, const Point& rhs)
	{
		return lhs.x < rhs.x;
	}
};

class POICorrespondence
{
public:

	static void findCorrespondences(const Mat &img1, vector<Point>& corners1, const Mat &img2,
			vector<Point>& corners2, map<Point, Point, PointCompare> &correspondences,
			float threshold, int WSize, IStrategyCompare *poiCompareMethod);
	static void plotCorrespondences(const Mat& img1, vector<Point>& corners1, const Mat& img2,
			map<Point, Point, PointCompare> &correspondences, Mat& dst);
};

} /* namespace poisim */
#endif /* POICORRESPONDENCE_H_ */
