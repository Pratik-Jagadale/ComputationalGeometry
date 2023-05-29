#pragma once

// GLEW
#include <gl/glew.h>

// OpenGL Header File
#include <GL/gl.h>

// GLEW
#pragma comment(lib, "glew32.lib")

// OpenGL Libraries
#pragma comment(lib, "OpenGL32.lib")

enum
{
	PRJ_ATTRIBUTE_POSITION = 0,
	PRJ_ATTRIBUTE_COLOR,
	PRJ_ATTRIBUTE_NORMAL,
	PRJ_ATTRIBUTE_TEXTURE0
};

enum GLShaderType
{
	VERTEX = 0,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};

