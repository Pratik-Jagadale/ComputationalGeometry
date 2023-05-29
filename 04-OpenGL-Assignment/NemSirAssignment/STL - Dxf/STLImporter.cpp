#include "STLImporter.h"
#include "GLLog.h"
#include "GLHeadersAndMacros.h"

using namespace std;

BOOL STLImporter::loadSTL(const char* fileName)
{
    // STL stl;

    std::string line;
    std::vector<std::string> lines;

    std::ifstream in(fileName);
    if (!in.is_open())
    {
        PrintLog("Cannot load model %s\n", fileName);
        return FALSE;
    }

    while (std::getline(in, line))
    {
        lines.push_back(line);
    }
    in.close();

    double a, b, c;  // Change the type to double for higher precision

    for (int i = 0; i < lines.size(); i++)
    {
        line = lines[i];

        if (line[0] == 'f')
        {
            sscanf_s(line.c_str(), "facet normal %lf %lf %lf", &a, &b, &c);
            for (int i = 0; i < 3; i++) // for per vertex normal
            {
                stl.normals.push_back(a);
                stl.normals.push_back(b);
                stl.normals.push_back(c);
            }
            //PrintLog("facet normal  %lf %lf %lf \n", a, b, c);
        }
        else if (line[2] == 'v')
        //else if (line.compare(0, 6) == "vertex")
        {
			// PrintLog("line.c_str() : %s\n", line.c_str());
			// PrintLog("line: %s\n", line.);

            sscanf(line.c_str(), "   vertex %le %le %le", &a, &b, &c);
            stl.vertices.push_back(a);
            stl.vertices.push_back(b);
            stl.vertices.push_back(c);
            PrintLog("vertex  %le %le %le \n", a, b, c);

            if (stl.vertices.size() == 3)
            {
                boundingBox.MAX_X = a;
                boundingBox.MAX_Y = b;
                boundingBox.MAX_Z = c;

                boundingBox.MIN_X = a;
                boundingBox.MIN_Y = b;
                boundingBox.MIN_Z = c;
            }
            else
            {
                if (a < boundingBox.MIN_X)
                {
                    PrintLog("boundingBox.MIN_X = %lf\n", boundingBox.MIN_X);
                    boundingBox.MIN_X = a;
                }
                if (a > boundingBox.MAX_X)
                    boundingBox.MAX_X = a;
                if (b < boundingBox.MIN_Y)
                {
                    PrintLog("boundingBox.MIN_Y = %lf\n", boundingBox.MIN_Y);
                    boundingBox.MIN_Y = b;
                }
                if (b > boundingBox.MAX_Y)
                    boundingBox.MAX_Y = b;
                if (c < boundingBox.MIN_Z)
                {
                    boundingBox.MIN_Z = c;
                    PrintLog("boundingBox.MIN_Z = %lf\n", boundingBox.MIN_Z);
                }
                if (c > boundingBox.MAX_Z)
                    boundingBox.MAX_Z = c;
            }
        }
    }
    lines.clear();
	return TRUE;
}

void BouningBox::GenerateBoundingBox(void)
{
PrintLog("MAX x = %lf , Min X = %lf\n", MAX_X,MIN_X);
PrintLog("MAX Y = %lf , Min Y = %lf\n", MAX_Y,MIN_Y);
PrintLog("MAX Z = %lf , Min Z = %lf\n", MAX_Z,MIN_Z);


GLdouble vertices[] = {
    MIN_X, MIN_Y, MIN_Z, 
     MAX_X, MIN_Y, MIN_Z,
     MAX_X, MAX_Y, MIN_Z,
    MIN_X, MAX_Y, MIN_Z, 
    MIN_X, MIN_Y, MAX_Z, 
     MAX_X, MIN_Y, MAX_Z,
     MAX_X, MAX_Y, MAX_Z,
    MIN_X,  MAX_Y, MAX_Z,
  };

GLuint indices[] = {
    0, 1, 2,  // Cube 1
    2, 3, 0,

    4, 5, 6,  // Cube 2
    6, 7, 4,

    0, 3, 7,  // Cube 3
    7, 4, 0,

    1, 2, 6,  // Cube 4
    6, 5, 1,

    3, 2, 6,  // Cube 5
    6, 7, 3,

    0, 1, 5,  // Cube 6
    5, 4, 0
};

	GLushort elements[] = {
    0, 1, 2, 3,
    4, 5, 6, 7,
    0, 4, 1, 5, 2, 6, 3, 7
  };

	double box[] = {
		// Front 
		MAX_X, MAX_Y, MAX_Z,
		MIN_X, MAX_Y, MAX_Z,
		MIN_X, MIN_Y, MAX_Z,
		MAX_X, MIN_Y, MAX_Z,

		// BACK
		MAX_X, MAX_Y, MIN_Z,
		MIN_X, MAX_Y, MIN_Z,
		MIN_X, MIN_Y, MIN_Z,
		MAX_X, MIN_Y, MIN_Z,

		// TOP
		MAX_X, MAX_Y, MIN_Z,
		MIN_X, MAX_Y, MIN_Z,
		MIN_X, MAX_Y, MAX_Z,
		MAX_X, MAX_Y, MAX_Z,

		// BOTTOM
		MAX_X, MIN_Y, MIN_Z,
		MIN_X, MIN_Y, MIN_Z,
		MIN_X, MIN_Y, MAX_Z,
		MAX_X, MIN_Y, MAX_Z,

		// LEFT
		MAX_X, MAX_Y, MIN_Z,
		MAX_X, MAX_Y, MAX_Z,
		MAX_X, MIN_Y, MAX_Z,
		MAX_X, MIN_Y, MIN_Z,

		// RIGHT
		MIN_X, MAX_Y, MIN_Z,
		MIN_X, MAX_Y, MAX_Z,
		MIN_X, MIN_Y, MAX_Z,
		MIN_X, MIN_Y, MIN_Z,

	};

	size = sizeof(indices) / sizeof(double);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vbo for position
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

BOOL STLImporter::loadDFX(const char* fileName)
{

	std::vector<Entity> entities;

	std::string line;
	std::vector<std::string> lines;

	std::ifstream in(fileName);
	if (!in.is_open())
	{
		PrintLog("Cannot load model %s\n", fileName);
		return FALSE;
	}

	while (!in.eof())
	{
		std::getline(in, line);
		lines.push_back(line);
	}
	in.close();

	double a, b, c;
	std::string entityType;
	for (int i = 0; i < lines.size(); i++)
	{
		line = lines[i];
	
			if (line == "ENTITIES")
			{
				while (i < lines.size()) {
					
				line = lines[i];
				if (line == "ENDSEC") {
					break; 
				}

				if(line == "  0"){

					Entity entity;
					entity.type = entityType;

					entities.push_back(entity);
				}
				else if(line == "LINE"){
					
				}
				else if(line == "CIRCLE"){
					
				}
				else if(line == "ARC"){
					
				}
				else if(line == "LWPOLYLINE"){
					
				}
				else if(line == "POLYLINE"){
					
				}
				else if(line == "SPLINE"){
					
				}
				else if(line == "POINT"){
					
				}

				i++;
			}
				
		}
	}
		lines.clear();

	return TRUE;
}


//int main(void){
//
//    BOOL bRead = loadSTL("sphere.stl");
//    if(bRead)
//    {
//        cout<< "STL File Read Successfully " <<endl;
//    }
//    else{
//        cout<< "Failed to Read STL FILE" <<endl;
//    }
//    return 0;
//}
