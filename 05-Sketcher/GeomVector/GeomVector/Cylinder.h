#pragma once

#include "Curve.h"

class Cylinder {
	Curve* cylinder;
public:
	Cylinder(int iCenterPoint, int iRadius, int iNumU);
	~Cylinder();
};
