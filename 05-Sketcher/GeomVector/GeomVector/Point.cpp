#include "pch.h"
#include "Point.h"

Point::Point()
{
	for (int i = 0; i < 3; i++)
	{
		point.assign(i, 0.0);
	}
}

Point::Point(double p1, double p2, double p3)
{
	point.assign(0, p1);
	point.assign(1, p2);
	point.assign(2, p3);
}

double Point::operator[](int index)
{
	return point[index];
}
void Point::operator()(int index, double value)
{
	if(index < 3)	
		point.assign(index, value);
}

void Point::Input(void)
{
	double temp= 0.0;

	cout << "Enter The Values:\n";
	for (int i = 0; i < 3; i++)
	{
		cin >> temp;
		point.assign(i, temp);
	}
}

void Point::Output(void)
{
	cout << point[0] << " " << point[1] << " " << point[2] << endl;
}

Point::~Point()
{

}
