
#define _USE_MATH_DEFINES
#include<stdio.h>
#include<cmath>

#include<vector>

FILE* gpFile;

struct Point{
    float x;
    float y;
    float z;
};

int main(void)
{
    int i = 0, j = 0;

    if(fopen_s(&gpFile, "Circle.dat", "w") != 0)
    {
        return -1;
    }
        float r = 1.01f;

        for(float angle = 0 ; angle <= 360.0f ; angle += 10.0f)
        {
            float rAngle = angle* M_PI/180.0f;

            float x = r * cos(rAngle);
            float y = r * sin(rAngle);
            fprintf(gpFile, "%f %f\n",x,y);
        }
    
    fclose(gpFile);

{
     if(fopen_s(&gpFile, "ParabolicCircle.dat", "w") != 0)
    {
        return -1;
    }
        r = 1.0f;

        float h = 1.0f;
        float k = 0.0f;

        std::vector<float> vec;

        Point point1, point2, point3;
        point1.x = 0.0f;
        point1.y = 0.2f;

        point2.x = 1.0f;
        point2.y = 0.2f;

        point3.x = 1.0f;
        point3.y = 0.2f;

        for (float u = 0; u <= 1.0f  ; u = u + 0.1f) 
        {
        float put_x = point1.x * pow((1-u),2) + 2*point1.x * (1-u) + point2.x * u; // Formula to draw curve
		float put_y = point1.y * pow((1-u),2) + 2*point1.y * (1-u) + point2.y * u;
		
		fprintf(gpFile, "%f %f\n",put_x,put_y);

        }
}
    fclose(gpFile);

    if(fopen_s(&gpFile, "QuadricBazierCurve.dat", "w") != 0)
    {
        return -1;
    }

    float Points[4][3] = {
        {-5.0f,5.0f,-5.0f},
        {5.0f,5.0f,0.0f},
        {5.0f,-5.0f,0.0f},
        {-5.0f,-5.0f,5.0f}
    };

    double put_x, put_y, put_z, t;
    
	for (t = 0.0; t <= 1.0; t = t + 0.1) // t always lies between 0 and 1
	{
		put_x = pow(1 - t, 3) * Points[0][0] + 3 * t * pow(1 - t, 2) * Points[1][0] + 3 * t * t * (1 - t) * Points[2][0] + pow(t, 3) * Points[3][0]; // Formula to draw curve
		put_y = pow(1 - t, 3) * Points[0][1] + 3 * t * pow(1 - t, 2) * Points[1][1] + 3 * t * t * (1 - t) * Points[2][1] + pow(t, 3) * Points[3][1];
		put_z = pow(1 - t, 3) * Points[0][2] + 3 * t * pow(1 - t, 2) * Points[1][2] + 3 * t * t * (1 - t) * Points[2][2] + pow(t, 3) * Points[3][2];

		fprintf(gpFile, "%f %f %f\n",put_x,put_y, put_z);
	}
    fclose(gpFile);


/*     if(fopen_s(&gpFile, "Line.dat", "w") != 0)
    {
        return -1;
    }

    Point p1, p2;
    p1.x = 0.0f;
    p1.y = 0.2f;

    p2.x = 1.0f;
    p2.y = 0.2f;

	for (float u = 0; u <= 1.0f  ; u = u + 0.1f) 
	{
		put_x = p1.x * (1-u) + p2.x * u;
		put_y = p1.y * (1-u) + p2.y * u;
		
		fprintf(gpFile, "%f %f\n",put_x,put_y);
	}
    fclose(gpFile);

 */

    if(fopen_s(&gpFile, "Surface.dat", "w") != 0)
    {
        return -1;
    }

    Point p1, p2, p3, p4;
    p1.x = 0.0f;
    p1.y = 0.0f;
    p1.z = 0.0f;

    p2.x = 5.0f;
    p2.y = 0.0f;
    p2.z = 0.0f;

    p3.x = 0.0f;
    p3.y = 2.0f;
    p3.z = 2.0f;

    p4.x = 5.0f;
    p4.y = 2.0f;
    p4.z = 2.0f;

	for (float u = 0; u <= 1.0f  ; u = u + 0.1f) 
	{
		float x1 = p1.x * (1-u) + p2.x * u;
		float y1 = p1.y * (1-u) + p2.y * u;
		float z1 = p1.z * (1-u) + p2.z * u;
            
        float x2 = p3.x * (1-u) + p4.x * u;
		float y2 = p3.y * (1-u) + p4.y * u;
		float z2 = p3.z * (1-u) + p4.z * u;

        float Points[4][3];
        float n = 0;
        for(int j = 0, n = 0.0f ; j < 4, n <= 1.0f; j++, n = n + 0.25f)
        {
              Points[j][0] = x1 * (1-n) + x2 * n;
              Points[j][1] = y1 * (1-n) + y2 * n;
              Points[j][2] = z1 * (1-n) + z2 * n;
        }

        for (t = 0.0; t <= 1.0; t = t + 0.1f) // t always lies between 0 and 1
        {
            put_x = pow(1 - t, 3) * Points[0][0] + 3 * t * pow(1 - t, 2) * Points[1][0] + 3 * t * t * (1 - t) * Points[2][0] + pow(t, 3) * Points[3][0]; // Formula to draw curve
            put_y = pow(1 - t, 3) * Points[0][1] + 3 * t * pow(1 - t, 2) * Points[1][1] + 3 * t * t * (1 - t) * Points[2][1] + pow(t, 3) * Points[3][1];
            put_z = pow(1 - t, 3) * Points[0][2] + 3 * t * pow(1 - t, 2) * Points[1][2] + 3 * t * t * (1 - t) * Points[2][2] + pow(t, 3) * Points[3][2];

            fprintf(gpFile, "%f %f %f\n",put_x,put_y, put_z);
        }

	}
    fclose(gpFile);


    return 0;
}

