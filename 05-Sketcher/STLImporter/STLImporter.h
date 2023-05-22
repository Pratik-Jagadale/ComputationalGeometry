#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>

using namespace std;

struct STL {
	vector<float> normals;
	vector<float> vertices;
};

struct Entity {
	std::string type;

};

class STLImporter
{
public:
	BOOL loadSTL(const char* fileName);
	BOOL loadDFX(const char* fileName);
};
