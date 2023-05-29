#ifndef _TRINGULATION_H
#define _TRINGULATION_H

#include "Vector.h"

struct VERTEX
{
	Vector3f Position;
	Vector3f Normal;
	Vector2f TexCoord;
};

struct FACE {
	VERTEX_T v1;
	VERTEX_T v2;
	VERTEX_T v3;
};

struct LINE {
	VERTEX_T v1;
	VERTEX_T v2;
};

typedef struct VERTEX VERTEX_T;
typedef struct FACE FACE_T;
typedef struct POINT LINE_T;

#endif