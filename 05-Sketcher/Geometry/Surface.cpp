#include "pch.h"
//#include "Surface.h"
//
//Surface::Surface()
//{
//
//}
//
//Curve Surface::operator[](int index)
//{
//    return surface[index];
//}
//
//Surface::Surface(Curve _c1, Curve _c2, int numV)
//{
//    for (float u = 0; u <= 1.0f; u = u + 0.1f)
//    {
//        float x1 = _c1[0] * (1 - u) + p2.x * u;
//        float y1 = p1.y * (1 - u) + p2.y * u;
//        float z1 = p1.z * (1 - u) + p2.z * u;
//
//        float x2 = p3.x * (1 - u) + p4.x * u;
//        float y2 = p3.y * (1 - u) + p4.y * u;
//        float z2 = p3.z * (1 - u) + p4.z * u;
//
//        float Points[4][3];
//        float n = 0;
//        for (int j = 0, n = 0.0f; j < 4, n <= 1.0f; j++, n = n + 0.25f)
//        {
//            Points[j][0] = x1 * (1 - n) + x2 * n;
//            Points[j][1] = y1 * (1 - n) + y2 * n;
//            Points[j][2] = z1 * (1 - n) + z2 * n;
//        }
//
//        for (t = 0.0; t <= 1.0; t = t + 0.1f) // t always lies between 0 and 1
//        {
//            put_x = pow(1 - t, 3) * Points[0][0] + 3 * t * pow(1 - t, 2) * Points[1][0] + 3 * t * t * (1 - t) * Points[2][0] + pow(t, 3) * Points[3][0]; // Formula to draw curve
//            put_y = pow(1 - t, 3) * Points[0][1] + 3 * t * pow(1 - t, 2) * Points[1][1] + 3 * t * t * (1 - t) * Points[2][1] + pow(t, 3) * Points[3][1];
//            put_z = pow(1 - t, 3) * Points[0][2] + 3 * t * pow(1 - t, 2) * Points[1][2] + 3 * t * t * (1 - t) * Points[2][2] + pow(t, 3) * Points[3][2];
//
//        }
//
//    }
//}
//
//vector<double >* Surface::getVertexData(void)
//{
//
//}
//
//int Surface::getVerticesCount(void)
//{
//
//}
//
//Surface::~Surface()
//{
//
//}
//
