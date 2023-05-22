#pragma once
#include "Point.h"
#include "Vector.h"
#include <vector>

class Line {

private:
	vector<Point> line;
	vector<double> *vertexData;
public:
	Line(Point p1, Point p2);
	vector<double>* getVertexdata(void);
	int len(void);
};