//============================================================================
// Name        : Ransac.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

struct PointCompare
{
	//This is extremely important for the map stl since the key-value
	//pair is <Point,Point>. Its used to locate
	bool operator()(const Point& me, const Point& other)
	{
		if (me.x != other.x)
			return me.x < other.x;
		return me.y < other.y;
	}

};

int normalize()
{

}

void loadCorrespondences(multimap<Point, Point, PointCompare> &matches)
{
	int N;
	int p1x, p1y, p2x, p2y;

	scanf("%d\n", &N);

	for(int i=0; i<N; i++)
	{
		scanf("%d %d %d %d\n", &p1x, &p1y, &p2x, &p2y);
		matches.insert(pair<Point,Point>(Point(p1x,p1y), Point(p2x,p2y)));
	};

	/*multimap<Point, Point, PointCompare>::iterator it;
	for(it=matches.begin(); it!=matches.end();++it)
		printf("[%3d,%3d] [%3d,%3d]\n",it->first.x, it->first.y, it->second.x, it->second.y);*/
}

int main()
{
	multimap<Point, Point, PointCompare> matches;
	loadCorrespondences(matches);

	return 0;
}
