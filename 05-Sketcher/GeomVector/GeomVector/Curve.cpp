#include "pch.h"
#include "Curve.h"

Curve::Curve()
{

}

Curve::Curve(int iNumU, Point p1, Point p2, Point p3, Point p4)
{
	for (int t = 0.0; t <= 1.0; t = t + 0.1) // t always lies between 0 and 1
	{
		
		double temp1 , temp2, temp3;

		temp1 = pow(1 - t, 3) * p1[0] + 3 * t * pow(1 - t, 2) * p2[0] + 3 * t * t * (1 - t) * p3[0] + pow(t, 3) * p4[0]; // Formula to draw curve
		temp2 = pow(1 - t, 3) * p1[1] + 3 * t * pow(1 - t, 2) * p2[1] + 3 * t * t * (1 - t) * p3[1] + pow(t, 3) * p4[1];
		temp3 = pow(1 - t, 3) * p1[2] + 3 * t * pow(1 - t, 2) * p2[2] + 3 * t * t * (1 - t) * p3[2] + pow(t, 3) * p4[2];
		Point temp_point(temp1, temp2, temp3);
		curve.push_back(temp_point);

		curveVertexData->push_back(temp1);
		curveVertexData->push_back(temp2);
		curveVertexData->push_back(temp3);
	}
}

Curve::Curve(int iCenterPoint, int iRadius, int iNumU)
{

}

Point Curve::operator[](int index)
{
	return curve[index];
}

void Curve::push_back(Point p)
{

}

Curve::~Curve()
{

}