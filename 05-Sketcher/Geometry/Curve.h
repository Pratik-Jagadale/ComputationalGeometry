#pragma once

#include "Point.h"
#include <vector>
#include "Export.h"

class DllExport Curve {

private:
	vector<Point> curve;
	vector<double>* curveVertexData;
public:
	Curve();
	Curve(int iNumU, Point p1, Point p2, Point p3, Point p4); // bazier curve
	Point operator[](int index);
	Curve(int iCenterPoint, int iRadius, int iNumU); // Parabola
	void push_back(Point p);
	~Curve();
};
