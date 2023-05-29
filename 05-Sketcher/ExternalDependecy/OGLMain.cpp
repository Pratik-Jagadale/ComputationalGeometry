/* Header Files */
#include <windows.h>
#include "OGLMain.h"
#include <stdio.h>
#include <stdlib.h>

#include "Sphere.h"

/* OpenGL Header files */
#include "vmath.h"
#include "GLShaders.h"
#include "GLLog.h"
#include "Noise.h"
#include "./3DModels/3DModels.h"

#include "UseSkyShader.h"
#include "UseWoodShader.h"
#include "UseMarbleShader.h"
#include "UseSunSurfaceShader.h"

#pragma comment(lib, "Sphere.lib")
#pragma comment(lib, "./3DModels/3DModels.lib")

using namespace vmath;

#define WINWIDTH 800
#define WINHEIGHT 600

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
BOOL gbActiveWindow = FALSE;

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;

// PP Related Global Variables
// class variables which coantins respective uniforms declartions and variables

CloudShader cloudShader_obj;
WoodShader woodShader_obj;
MarbleShader marbleShader_obj;
SunSurfaceShader sunSurfaceShader;

mat4 perspectiveProjectionMatrix;

// Cyilnder
GLuint vao_Cylinder;
GLuint vbo_Cylinder_Position;
GLuint vbo_Cylinder_Texcoord;
GLuint vbo_Cylinder_Normal;

// Cube
GLuint vao_Cube;
GLuint vbo_Cube_Position;
GLuint vbo_Cube_Texcoord;
GLuint vbo_Cube_Normal;

// Torus
GLuint vao_Torus;
GLuint vbo_Torus_Position;
GLuint vbo_Torus_Texcoord;
GLuint vbo_Torus_Normal;

// Pyramid
GLuint vao_Pyramid;
GLuint vbo_Pyramid_Position;
GLuint vbo_Pyramid_Texcoord;
GLuint vbo_Pyramid_Normal;

// SPHERE
GLuint vao_Sphere;
GLuint vbo_Sphere_Position;
GLuint vbo_Sphere_Texcoord;
GLuint vbo_Sphere_Normal;
GLuint gVbo_Sphere_element;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

vec3 eye = vec3(0.0f, 0.0f, 6.0f);
vec3 center = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

Cylinder *cylinder = nullptr;
Torus *torus = nullptr;
Pyramid *pyramid = nullptr;
// Cube cube;

/* MARBLE RELATED VARIABLES */
GLuint texture_noise;
GLuint texture_noise_cloud;

GLfloat scaleFactor = 2.0f;

/* Entry Point Function */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	/* function declartions */
	int initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	/* variable declarations */
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyWindow");
	BOOL bDone = FALSE;
	int iRetVal = 0;
	int iHeightOfWindow, iWidthOfWindow;

	// Code
	CreateLogFile();

	// Initializaion of wndclassex structure
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	/* Register Above wndclass */
	RegisterClassEx(&wndclass);

	iHeightOfWindow = GetSystemMetrics(SM_CYSCREEN); // Height of Window Screen
	iWidthOfWindow = GetSystemMetrics(SM_CXSCREEN);	 // Width Of Window Screen

	/* Create Window */
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, szAppName,
						  TEXT("OpenGL -  Pratik Rajendra Jagadale"),
						  WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
						  (iWidthOfWindow - WINWIDTH) / 2,
						  (iHeightOfWindow - WINHEIGHT) / 2,
						  WINWIDTH,
						  WINHEIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ghwnd = hwnd;

	// initizalize
	iRetVal = initialize();

	if (iRetVal == -1)
	{
		PrintLog("Choose Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		PrintLog("Set Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		PrintLog("Crete OpenGl Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -4)
	{
		PrintLog("Makeing OpenGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		PrintLog("GLEW Initialization Failed...\n");
		uninitialize();
	}
	else
	{
		PrintLog("Initialize Successfull...\n");
	}

	ShowWindow(hwnd, iCmdShow);

	/* fore grounding and focusing window */
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	/* Game Loop */
	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == TRUE)
			{
				/* Render the seen */
				display();
				// updatetheseen
				update();
			}
		}
	}

	uninitialize();
	return ((int)msg.wParam);
}

/* CALLBACK Function */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	/* fucntion declarations */
	// void ToggleFullScreen();
	void resize(int, int);

	// code
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActiveWindow = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActiveWindow = FALSE;
		break;

	case WM_ERASEBKGND:
		return (0);

	case WM_CHAR:
		switch (wParam)
		{
		case 'f':
		case 'F':
			ToggleFullScreen();
			break;

		case 'w':
			eye[2] = eye[2] - 0.1f;
			// center[2] = center[2] - 0.1f;
			break;

		case 's':
			eye[2] = eye[2] + 0.1f;
			// center[2] = center[2] + 0.01f;
			break;

		case 'a':
			eye[0] = eye[0] - 0.1f;
			// center[0] = center[0] - 0.1f;

			break;

		case 'd':
			eye[0] = eye[0] + 0.1f;
			// center[0] = center[0] + 0.1f;
			break;

		case 'q':

			eye[1] = eye[1] + 0.1f;
			// center[1] = center[1] + 0.1f;
			break;

		case 'e':
			eye[1] = eye[1] - 0.1f;
			//  center[1] = center[1] - 0.1f;
			break;

			/*
					case 'w':
						if (alphaSky <= 1.0f)
							alphaSky = alphaSky + 0.1f;
						break;

					case 'W':
						if (alphaSky >= 0.0f)
							alphaSky = alphaSky - 0.1f;
						break;

					case 's':
						if (alphaCloud <= 1.0f)
							alphaCloud = alphaCloud + 0.1f;
						break;

					case 'S':
						if (alphaCloud >= 0.0f)
							alphaCloud = alphaCloud + 0.1f;
						break;

						break;
			*/
		case 'l':
		case 'L':
			break;

		case 27:
			PostQuitMessage(0);
		}
		break;

	case WM_SIZE:
		resize(WORD(lParam), HIWORD(lParam));
		break;

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullScreen()
{
	// variable declartions
	static DWORD dwStyle;
	static WINDOWPLACEMENT wp;
	MONITORINFO mi;

	//	code
	wp.length = sizeof(WINDOWPLACEMENT);
	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi.cbSize = sizeof(MONITORINFO);

			if (GetWindowPlacement(ghwnd, &wp) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED); // nccalksize
			}

			ShowCursor(FALSE);
			gbFullScreen = TRUE;
		}
	}
	else
	{
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);

		SetWindowPlacement(ghwnd, &wp);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
		gbFullScreen = FALSE;
	}
}

int initialize(void)
{
	/* fucntion delcations */
	void resize(int, int);
	void uninitialize(void);

	/* variable declartions */
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = 0;

	/* code */
	/* initialization of pixelformatdesciptor structure */
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR)); // memset((void*)&pfd , NULL, sizeof(OIXELFORAMTEDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;

	/* GetDC */
	ghdc = GetDC(ghwnd);

	/* Choose Pixel Format */
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);

	if (iPixelFormatIndex == 0)
		return -1;

	/* Set The choosen Puxel Format */
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
		return -2;

	/* binding API */
	/* Create OpenGL Rendering Context */
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
		return -3;

	/* make the rendering as current cintext */
	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
		return -4;

	/* Here start OpeGL Code */
	// GLEW INITIALIZATION
	if (glewInit() != GLEW_OK)
		return -5;

	// Print OpenGL Info
	PrintGLInfo();

	BOOL status = woodShader_obj.initialize_WoodShaderObject();

	status = cloudShader_obj.initialize_cloudShaderObject();

	status = marbleShader_obj.initialize_marbleShaderObject();

	status = sunSurfaceShader.initialize_SunSurafaceShaderObject();

	// Geometry Object Creation
	cylinder = new Cylinder(1.0f, 1.0f, 5.0f, 100.0f, 100.0f);

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// Cylinder vao & vbo
	//  vao_Cylinder
	glGenVertexArrays(1, &vao_Cylinder);
	glBindVertexArray(vao_Cylinder);

	// vbo for position
	glGenBuffers(1, &vbo_Cylinder_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cylinder_Position);
	glBufferData(GL_ARRAY_BUFFER, cylinder->vertices->size * sizeof(float), cylinder->vertices->p_arr, GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normal
	glGenBuffers(1, &vbo_Cylinder_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cylinder_Normal);
	glBufferData(GL_ARRAY_BUFFER, cylinder->normals->size * sizeof(float), cylinder->normals->p_arr, GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Sphere vao and vbo related code
	// vao_Sphere
	glGenVertexArrays(1, &vao_Sphere);
	glBindVertexArray(vao_Sphere);

	// vbo for position
	glGenBuffers(1, &vbo_Sphere_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere_Position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normal
	glGenBuffers(1, &vbo_Sphere_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Sphere_Normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_Sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Cube vao & vbo
	//  vao_Cube
	/*glGenVertexArrays(1, &vao_Cube);
	glBindVertexArray(vao_Cube);

	// vbo for position
	glGenBuffers(1, &vbo_Cube_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Position);
	glBufferData(GL_ARRAY_BUFFER, cube.getVerticesCount() * sizeof(float), cube.getVerticesData(), GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normal
	glGenBuffers(1, &vbo_Cube_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Normal);
	glBufferData(GL_ARRAY_BUFFER, cube.getNormalsCount() * sizeof(float), cube.getNormalsData(), GL_STATIC_DRAW);
	glVertexAttribPointer(PVG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
*/
	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	// Enabaling the texture
	glEnable(GL_TEXTURE_3D);

	/* Clear the  screen using blue color */
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	CreateNoise3D(&texture_noise);
	CreateNoise3D(&texture_noise_cloud);

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);

	return (0);
}

void resize(int width, int height)
{
	/* code */
	if (height == 0) // to avoid devided by zero
		height = 1;

	glViewport(0, 0, width, height);

	perspectiveProjectionMatrix = vmath::perspective(
		45.0f,
		(GLfloat)width / (GLfloat)height,
		0.1f,
		100.0f);
}

void display(void)
{
	/* Code */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use shader program obejct
	glUseProgram(sunSurfaceShader.shaderProgramObject);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	viewMatrix = vmath::lookat(eye, center, up);
	translationMatrix = vmath::translate(1.2f, 0.0f, 0.0f);
	rotateMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	modelMatrix = translationMatrix * scaleMatrix;

	// modelMatrix = translationMatrix * rotateMatrix;

	glUniformMatrix4fv(sunSurfaceShader.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(sunSurfaceShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(sunSurfaceShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	sunSurfaceShader.update_SunSurfaceObjectUniforms();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_noise);
	glUniform1i(sunSurfaceShader.textureSamplerUniform, 0);
	/*
		glBindVertexArray(vao_Cylinder);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, cylinder->vertices->size);
		glBindVertexArray(0);
	*/

	glBindVertexArray(vao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	// unuse the shader program object
	glUseProgram(0);

	//============================================================================

	// use shader program obejct
	glUseProgram(cloudShader_obj.shaderProgramObject);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(-1.2f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(cloudShader_obj.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(cloudShader_obj.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(cloudShader_obj.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	cloudShader_obj.update_CloudShaderObjectUniforms();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_noise_cloud);
	glUniform1i(cloudShader_obj.textureSamplerUniform, 0);

	glBindVertexArray(vao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	//============================================================================

	// use shader program obejct
	glUseProgram(marbleShader_obj.shaderProgramObject);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(-3.5f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(marbleShader_obj.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(marbleShader_obj.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(marbleShader_obj.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	marbleShader_obj.update_CloudShaderObjectUniforms();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_noise_cloud);
	glUniform1i(marbleShader_obj.textureSamplerUniform, 0);

	glBindVertexArray(vao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

	// ================= WOOD ==========================
	//============================================================================

	// use shader program obejct
	glUseProgram(woodShader_obj.shaderProgramObject);

	translationMatrix = mat4::identity();
	modelMatrix = mat4::identity();

	translationMatrix = vmath::translate(3.5f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	modelMatrix = translationMatrix * scaleMatrix;

	glUniformMatrix4fv(woodShader_obj.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(woodShader_obj.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(woodShader_obj.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	woodShader_obj.update_WoodShaderObjectUniforms();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, texture_noise_cloud);
	glUniform1i(woodShader_obj.textureSamplerUniform, 0);

	glBindVertexArray(vao_Sphere);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_Sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */

	// skyColor[3] = alphaSky;
	// cloudColor[3] = alphaCloud;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	free(cylinder);

	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	if (vbo_Sphere_Normal)
	{
		glDeleteBuffers(1, &vbo_Sphere_Normal);
		vbo_Sphere_Normal = 0;
	}

	// deletion of vbo_Sphere_Texcoord
	if (vbo_Sphere_Texcoord)
	{
		glDeleteBuffers(1, &vbo_Sphere_Texcoord);
		vbo_Sphere_Texcoord = 0;
	}

	// deletion of vbo_Sphere_Position
	if (vbo_Sphere_Position)
	{
		glDeleteBuffers(1, &vbo_Sphere_Position);
		vbo_Sphere_Position = 0;
	}

	// deletion of vao_Sphere
	if (vao_Sphere)
	{
		glDeleteVertexArrays(1, &vao_Sphere);
		vao_Sphere = 0;
	}

	if (vbo_Cylinder_Normal)
	{
		glDeleteBuffers(1, &vbo_Cylinder_Normal);
		vbo_Cylinder_Normal = 0;
	}

	// deletion of vbo_Color
	if (vbo_Cylinder_Texcoord)
	{
		glDeleteBuffers(1, &vbo_Cylinder_Texcoord);
		vbo_Cylinder_Texcoord = 0;
	}

	// deletion of vbo_Cylinder_Position
	if (vbo_Cylinder_Position)
	{
		glDeleteBuffers(1, &vbo_Cylinder_Position);
		vbo_Cylinder_Position = 0;
	}

	// deletion of vao_Cylinder
	if (vao_Cylinder)
	{
		glDeleteVertexArrays(1, &vao_Cylinder);
		vao_Cylinder = 0;
	}

	UninitializeShaders(woodShader_obj.shaderProgramObject);
	UninitializeShaders(cloudShader_obj.shaderProgramObject);
	UninitializeShaders(marbleShader_obj.shaderProgramObject);
	UninitializeShaders(sunSurfaceShader.shaderProgramObject);

	if (wglGetCurrentContext() == ghrc)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc)
	{
		ReleaseDC(ghwnd, ghdc);
		ghwnd = NULL;
		ghdc = NULL;
	}

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
	}

	CloseLogFile();
}
