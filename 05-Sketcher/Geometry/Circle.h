#pragma once

#include "Point.h"
#include "Curve.h"
#include "Vector.h"
#include <vector>

class Circle
{
private:
    Curve* circle = NULL;

public:
    vector<double> circleVertexData;
    Circle(Point center, int iRadius, int iNumU);
    vector<double> getVertexData(void);
    int getVertexCount(void);
    ~Circle();
};


