#pragma once


#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <string>
#include "GLHeadersAndMacros.h"

using namespace std;

struct Triangle{
	
};

struct STL {
	vector<float> normals;
	vector<float> vertices;
};

struct Entity {
	std::string type;
};

class BouningBox{
public:
	double MAX_X;
	double MIN_X;

	double MAX_Y;
	double MIN_Y;

	double MAX_Z;
	double MIN_Z;

	// vector<float> vertices;
	int size;
	GLuint vao, vbo,ibo;

	//BouningBox();
	void GenerateBoundingBox(void);

};

class STLImporter
{
public:
	struct STL stl;
	struct BouningBox boundingBox;
	

	BOOL loadSTL(const char* fileName);
	BOOL loadDFX(const char* fileName);
};
