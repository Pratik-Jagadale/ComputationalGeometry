#include "pch.h"
#include "Line.h"

Line::Line(Point p1, Point p2)
{
	for (float u = 0; u <= 1.0f; u = u + 0.1f)
	{
		double put_x = p1[0] * (1 - u) + p2[0] * u;
		double put_y = p1[1] * (1 - u) + p2[1] * u;
		double put_z = 0.0;
		
		line.push_back(Point(put_x, put_y, put_z));

		vertexData->push_back(put_x);
		vertexData->push_back(put_y);
		vertexData->push_back(put_z);
		
	}
}

vector<double>* Line::getVertexdata(void)
{
	return (vertexData);
}

int Line::len(void)
{
	return (vertexData->size());
}
