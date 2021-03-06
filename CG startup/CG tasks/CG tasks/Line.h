#pragma once
#ifndef LINE
#define LINE

#include "Point.h"
class Line
{
public:
	static int drawID;
	int lineDrawID;
	Point start, end;
	Line(){}
	Line(Point startPoint, Point endPoint, bool toDraw=false): start(startPoint), end(endPoint)
	{
		if(toDraw) lineDrawID = drawID++;
		else lineDrawID = 0;
	}

};
#endif