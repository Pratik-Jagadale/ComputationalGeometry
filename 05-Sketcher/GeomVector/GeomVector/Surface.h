#pragma once

#include "Curve.h"

class Surface {
private:
	vector<Curve > surface;
	vector<double >* surfaceVertexData;
public:
	Surface();
	Surface(Curve _c1, Curve _c2, int numV);
	Curve operator[](int index);
	vector<double >* getVertexData(void);
	int getVerticesCount(void);
	~Surface();
};