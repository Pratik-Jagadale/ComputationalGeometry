#include "pch.h"
#include "STLImporter.h"

using namespace std;


BOOL STLImporter::loadSTL(const char* fileName)
{
    STL stl;

	std::string line;
	std::vector<std::string> lines;

	std::ifstream in(fileName);
	if (!in.is_open())
	{
		printf("Cannot load model %s\n", fileName);
		return FALSE;
	}

	while (!in.eof())
	{
		std::getline(in, line);
		lines.push_back(line);
	}
	in.close();

	double a, b, c;
	for (int i = 0; i < lines.size(); i++)
	{
        //cout<< "lines.size()" <<lines.size() <<endl;
		line = lines[i];
        // cout<< line <<endl;
		//if (line[0] == 'v')
		//{
			if (line[0] == 'f')
			{
				sscanf_s(line.c_str(), "facet normal %lf %lf %lf", &a, &b, &c);
				//vertices.push_back(new float[3]{ a, b, c });
				for (int i = 0; i < 3; i++) // for per vertex normal
				{
					stl.normals.push_back(a);
					stl.normals.push_back(b);
					stl.normals.push_back(c);
 				}
                cout<<"facet normal " <<a <<" " <<b <<" " <<c <<endl;

			}
			// else if (line == "        vertex")
			//else if (strcmp(line.c_str(), "vertex") < 0)
			else if (line[2] == 'v')
			{
				sscanf_s(line.c_str(), "        vertex %lf %lf %lf", &a, &b, &c);
				//texcoords.push_back(new float[2]{ a, b });
				stl.vertices.push_back(a);
				stl.vertices.push_back(b);
				stl.vertices.push_back(c);
                cout<<"        vertex " <<a <<" " <<b <<" " <<c <<endl;
			}
		//}
		
	}
	lines.clear();

	return TRUE;
}



BOOL STLImporter::loadDFX(const char* fileName)
{

	std::vector<Entity> entities;

	std::string line;
	std::vector<std::string> lines;

	std::ifstream in(fileName);
	if (!in.is_open())
	{
		printf("Cannot load model %s\n", fileName);
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
