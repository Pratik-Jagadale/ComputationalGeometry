#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>

#include "Export.h"

using namespace std;

struct STL {
	vector<float> normals;
	vector<float> vertices;
};

struct Entity {
	std::string type;

};

class DllExport STLImporter
{
	STL stl;

public:
	BOOL loadSTL(const char* fileName);
	BOOL loadDFX(const char* fileName);
};
