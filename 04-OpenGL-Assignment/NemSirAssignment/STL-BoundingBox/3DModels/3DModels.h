#pragma once

#ifdef MAKEDLL
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

//#define _USE_MATH_DEFINES

typedef struct vec_float vec_float_t;
struct vec_float
{
    float *p_arr;
    int size;
};

void getCircleVertexData(float *circlePositionCoords);
unsigned int getNumberOfCircleVertices(void);
void deleteCircleVertexData(float *circlePositionCoords);

const int MAX_VERTICES = 100000;
const int MAX_TEXCOORDS = 100000;
const int MAX_NORMALS = 100000;
const int CACHE_SIZE = 240;
const int MAX_ARRAY_SIZE = 100000;


class EXPORT HemiSphere
{
private:
    void addSphereVertex(double x, double y, double z);
    void addSphereNormal(double x, double y, double z);

    void addSphereTexCoords(double a, double b);

public:
    // variables
    float model_vertices[MAX_VERTICES];
    float model_textures[MAX_TEXCOORDS];
    float model_normals[MAX_NORMALS];
    int texturePointer = 0;
    int verticesPointer = 0;
    int normalsPointer = 0;

    // methods
    HemiSphere(float radius, int slices, int stacks, BOOL _isFront, float heightOfCylinder);

    HemiSphere(float radius, int slices, int stacks);
    
    float* getSphereVertex();
    float* getSphereTexCoord();
    float* getSphereNormals();

    // Function which generates sphere coordinates
    void generateSphereData(float radius, int slices, int stacks, BOOL _isFront, float heightOfCylinder);

    int getNumberOfSphereVertices();

    int getNumberOfSphereTexcoords();

    int getNumberOfSphereNormals();
};

class EXPORT Cylinder
{
private:
    float baseRadius;
    float topRadius;
    float height;
    float sector;
    float stack;

    vec_float *unitCircleVertices;
    void AddVertex(float x, float y, float z);

    void AddNormal(float x, float y, float z);

    void AddTexCoord(float s, float t);

    void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);

    vec_float* GetSideNormals();

    void GenerateUnitCircleVertices();

    void GenerateVertices();
    void GenerateVerticesFlat();

    std::vector<float> computeFaceNormal(float x1, float y1, float z1, // v1
        float x2, float y2, float z2, // v2
        float x3, float y3, float z3);


public:
    vec_float *vertices;
    vec_float *normals;
    vec_float *texCoords;
    vec_float *indices;

    Cylinder(float _baseRadius, float _topRadius, float _height, float _stack, float _sector);

    ~Cylinder();

    
    
};

class EXPORT Cone
{


public :

    Cylinder* cylinder = NULL;

    float* getConeVertex();
    float* getConeTexCoord();
    float* getConeNormals();

    int getNumberOfConeVertices();
    int getNumberOfConeTexcoords();
    int getNumberOfConeNormals();

    Cone(float Radius, float Height, float stack, float slices);
    ~Cone();
};

class EXPORT Capsule
{
private:
    float radius;
    float height;
    HemiSphere* hemiSpherefFront = NULL;
    HemiSphere* hemiSphereBack = NULL;
    Cylinder* cylinder = NULL;

    void InitiallizeCapsule(float _radius, float _height, float slice, float stack);
    void GenereateCapsule();

public:
    
    vec_float* vertices;
    vec_float* texCoords;
    vec_float* normals;
    vec_float* indices;

    Capsule();

    ~Capsule();
};

class EXPORT Sphere
{
private:
    void addTexcoord(float a, float b);
    void addVertices(float x, float y, float z);
    void addNormals(float p, float q, float r);
    void generateSphereData1(double radius, int slices, int stacks);
    float model_Vertices[MAX_ARRAY_SIZE];
    float model_Texcoord[MAX_ARRAY_SIZE];
    float model_Normal[MAX_ARRAY_SIZE];

    int VertexPointer = 0;
    int TexcoordPointer = 0;
    int NormalPointer = 0;

public:
    

    Sphere(double radius, int slices, int stacks);
    int getNumberOfSphereVertices(void);
    int getNumberOfSphereTexcoord(void);
    int getNumberOfSphereNormal(void);
    float* getSphereVertex(void);
    float* getSphereTexcoord(void);
    float* getSphereNormal(void);
    ~Sphere();
};

class EXPORT Torus
{
public:
    // variables
    float* model_vertices;
    float* model_normals;
    float* model_textures;

    int torusCoordsPointer = 0;
    int torusTexCoordsPointer = 0;
    int torusNormalsPointer = 0;
    // methods

    Torus();

    void addTorusVertex(double x, double y, double z);

    void addTorusNormal(double x, double y, double z);

    void addTorusTexCoords(double a, double b);

    float* getTorusVertex();

    float* getTorusTexCoord();
    float* getTorusNormals();

    void generateTorusData();
    int getNumberOfTorusVertices();

    int getNumberOfTorusTexcoords();

    int getNumberOfTorusNormals();

    ~Torus();
};

class EXPORT Disk
{
public:
    // variables
    float model_Vertices[MAX_VERTICES];
    float model_Texcoord[MAX_TEXCOORDS];
    float model_Normal[MAX_NORMALS * 10];

    int VertexPointer = 0;
    int TexcoordPointer = 0;
    int NormalPointer = 0;
    // methods

    Disk(double innerRadius,
        double outerRadius, int slices, int loops);

    void addTexcoord(float a, float b);

    void addVertices(float x, float y, float z);

    void addNormals(float p, float q, float r);

    int getNumberOfDiskVertices();

    int getNumberOfDiskTexcoord();

    int getNumberOfDiskNormal();

    float* getDiskVertex();

    float* getDiskTexcoord();

    float* getDiskNormal();

    void generateDiskData(double innerRadius,
        double outerRadius, int slices, int loops);

};

class EXPORT Pyramid
{
private:
    int pyramidPositionCount = 36;
    int pyramidNormalCount = 36;
    int pyramidTexcoordCount = 24;

public:
    float pyramidPosition[36] =
    {
        // front
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        // right
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        // back
        0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        // left
        0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };

    float pyramidNormals[36] =
    {
        0.0f, 0.447214f, 0.894427f,// front-top
        0.0f, 0.447214f, 0.894427f,// front-left
        0.0f, 0.447214f, 0.894427f,// front-right

        0.894427f, 0.447214f, 0.0f, // right-top
        0.894427f, 0.447214f, 0.0f, // right-left
        0.894427f, 0.447214f, 0.0f, // right-right

        0.0f, 0.447214f, -0.894427f, // back-top
        0.0f, 0.447214f, -0.894427f, // back-left
        0.0f, 0.447214f, -0.894427f, // back-right

        -0.894427f, 0.447214f, 0.0f, // left-top
        -0.894427f, 0.447214f, 0.0f, // left-left
        -0.894427f, 0.447214f, 0.0f // left-right

    };

    float pyramidTexcoord[24] =
    {
        0.5, 1.0, // front-top
        0.0, 0.0, // front-left
        1.0, 0.0, // front-right

        0.5, 1.0, // right-top
        1.0, 0.0, // right-left
        0.0, 0.0, // right-right

        0.5, 1.0, // back-top
        1.0, 0.0, // back-left
        0.0, 0.0, // back-right

        0.5, 1.0, // left-top
        0.0, 0.0, // left-left
        1.0, 0.0, // left-right
    };

    Pyramid();
    float* getVerticesData();

    float* getNormalsData();
    float* getTexcooardData();

    int getVerticesCount();

    int getNormalsCount();

    int getTexcoordsCount();

};

class EXPORT Cube
{
private:
    int cubePositionCount = 72;
    int cubeNormalCount = 72;
    int cubeTexcoordCount = 48;

public:
    float cubePosition[72] =
    {
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
       -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        
        1.0f, 1.0f, 1.0f,
       -1.0f, 1.0f, 1.0f,
       -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        
        1.0f, 1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f
    };

    float cubeNormals[72] =
    {
        // top surface
        0.0f, 1.0f, 0.0f,  // top-right of top
        0.0f, 1.0f, 0.0f, // top-left of top
        0.0f, 1.0f, 0.0f, // bottom-left of top
        0.0f, 1.0f, 0.0f,  // bottom-right of top

        // bottom surface
        0.0f, -1.0f, 0.0f,  // top-right of bottom
        0.0f, -1.0f, 0.0f,  // top-left of bottom
        0.0f, -1.0f, 0.0f,  // bottom-left of bottom
        0.0f, -1.0f, 0.0f,   // bottom-right of bottom

        // front surface
        0.0f, 0.0f, 1.0f,  // top-right of front
        0.0f, 0.0f, 1.0f, // top-left of front
        0.0f, 0.0f, 1.0f, // bottom-left of front
        0.0f, 0.0f, 1.0f,  // bottom-right of front

        // back surface
        0.0f, 0.0f, -1.0f,  // top-right of back
        0.0f, 0.0f, -1.0f, // top-left of back
        0.0f, 0.0f, -1.0f, // bottom-left of back
        0.0f, 0.0f, -1.0f,  // bottom-right of back

        // right surface
        1.0f, 0.0f, 0.0f,  // top-right of right
        1.0f, 0.0f, 0.0f,  // top-left of right
        1.0f, 0.0f, 0.0f,  // bottom-left of right
        1.0f, 0.0f, 0.0f,  // bottom-right of right

        // left surface
        -1.0f, 0.0f, 0.0f, // top-right of left
        -1.0f, 0.0f, 0.0f, // top-left of left
        -1.0f, 0.0f, 0.0f, // bottom-left of left
        -1.0f, 0.0f, 0.0f // bottom-right of left

    };

    float cubeTexcoord[48] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    Cube();
    ~Cube();
    
    float* getVerticesData(void);
    float* getNormalsData(void);
    float* getTexcooardData(void);

    int getVerticesCount(void);
    int getNormalsCount(void);
    int getTexcoordsCount(void);

};
