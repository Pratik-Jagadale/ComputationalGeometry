#pragma once

#include "vmath.h"
#include "GLLog.h"
#include "GLHeadersAndMacros.h"
#include <cmath>

void init3DNoiseTexture(int texSize, GLubyte *texPtr);
void make3DNoiseTexture();

void CreateNoise3D(GLuint *);
void SetNoiseFrequency(int frequency);

double noise1(double arg);
double noise2(double vec[2]);
double noise3(double vec[3]);

void normalize2(double v[2]);
void normalize3(double v[3]);
void initNoise();

// My harmonic summing functions - PDB

//
// In what follows "alpha" is the weight when the sum is formed.
// Typically it is 2, As this approaches 1 the function is noisier.
// "beta" is the harmonic scaling/spacing, typically 2.
//

double PerlinNoise1D(double x, double alpha, double beta, int n);
double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);
void make3DNoiseTexture();
void init3DNoiseTexture(int texSize, GLuint *texPtr);
