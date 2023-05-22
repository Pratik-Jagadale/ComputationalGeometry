#include "pch.h"
#include "Circle.h"

Circle::Circle(Point center, int iRadius, int iNumU)
{
    if (circle == nullptr)
        circle = new Curve();

    double uAngle = 360 / iNumU;
    for (double angle = 0; angle <= 360.0; angle += uAngle)
    {
        double rAngle = angle * M_PI / 180.0;

        double x = (iRadius * cos(rAngle)) + center[0];
        double y = iRadius * sin(rAngle) + center[1];
        double z = center[2];
        cout << x << " " << y << " " << z << endl;
        circle->push_back(Point(x, y, z));
    }

}

vector<double> Circle::getVertexData(void)
{
    return circleVertexData;
}

int Circle::getVertexCount(void)
{
    return circleVertexData.size();
}

Circle::~Circle()
{

}
