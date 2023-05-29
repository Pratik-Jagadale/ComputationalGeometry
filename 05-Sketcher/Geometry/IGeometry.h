#pragma once
#include<iostream>
#include<string>
#include "Export.h"

class IGeometry
{
private:

public:
	std::string name;
	IGeometry() {};
	IGeometry(std::string name) {
		this->name = name;
	}
	virtual void input() = 0;
	virtual void display() = 0;

};
