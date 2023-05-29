#pragma once

#include<iostream>
#include "Vector.h"
#define _USE_MATH_DEFINES
#include<cmath>
#include<fstream>

using namespace std;

class Point{

 private:
     // double data[3];
     Vector3f point;

 public:  
      Point();
      Point(double p1, double p2, double p3);
      double operator[](int index);
      
      void operator()(int index, double value);
      void Input(void);
      void Output(void);
      ~Point();
 };
