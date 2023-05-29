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

#pragma comment(lib, "Sphere.lib")

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
GLuint shaderProgramObject;

GLuint vao;			 // Vertex Array Object -
GLuint vbo_Position; // Vertex Buffer Object - Pyramid - Position
GLuint vbo_Texcoord; // Vertex Buffer Object - Pyramid
GLuint vbo_Normal;
GLuint gVbo_sphere_element;

GLuint projectMatrixUniform; // model View Projection
GLuint viewMatrixUniform;	 // model View Projection
GLuint modelMatrixUniform;	 // model View Projection
mat4 perspectiveProjectionMatrix;

GLuint lightPosUniform;
GLuint scaleUniform;
GLuint skyColorUniform;
GLuint cloudColorUniform;

GLuint textureSamplerUniform;
GLuint texture_noise;
GLubyte *myNoise3DTexPtr;

float lightPos[4] = {100.0f, 100.0f, 100.0f, 1.0f};

float skyColor[3] = {0.0f, 0.0f, 0.8f};
float cloudColor[3] = {0.8f, 0.8, 0.8f};

// float skyColor[3] = {0.7f, 0.7f, 0.7f}; // marble
// float cloudColor[3] = {0.0f, 0.15f, 0.0f};// marble

GLfloat scaleFactor = 1.0f;
BOOL isScaled = FALSE;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
int gNumVertices;
int gNumElements;

float alphaSky = 1.0f;
float alphaCloud = 1.0f;
BOOL glAlphaEnable = FALSE;

struct woodUniform
{
	GLuint LightWoodColor;
	GLuint DarkWoodColor;
	GLuint RingFreq;
	GLuint LightGrains;
	GLuint DarkGrains;
	GLuint GrainThreshold;
	GLuint NoiseScale;
	GLuint Noisiness;
	GLuint GrainScale;
};
float wood_LightWoodColor_Factor[3];
float wood_DarkWoodColor_Factor[3];
float wood_RingFreq_Factor;
float wood_LightGrains_Factor;
float wood_DarkGrains_Factor;
float wood_GrainThreshold_Factor;
float wood_NoiseScale_Factor[3];
float wood_Noisiness_Factor;
float wood_GrainScale_Factor;

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

		case 'l':
		case 'L':
			if (glAlphaEnable == TRUE)
			{
				glAlphaEnable = FALSE;
			}
			else
			{
				glAlphaEnable = TRUE;
			}

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

	// vertex Shader
	GLuint vertexShaderObject = CreateAndCompileShaderObjects("Shaders\\cloud.vs", VERTEX);

	// fragment Shader
	GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\wood.fs", FRAGMENT);
	// GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\cloud.fs", FRAGMENT);
	//  GLuint fragmentShaderObject = CreateAndCompileShaderObjects("Shaders\\marble.fs", FRAGMENT);

	// Shader Program Object
	// pr
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);
	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// prelinked binding
	// Binding Position Array
	glBindAttribLocation(shaderProgramObject, PVG_ATTRIBUTE_POSITION, "a_position");
	// Binding Color Array
	glBindAttribLocation(shaderProgramObject, PVG_ATTRIBUTE_NORMAL, "a_normal");

	// link
	BOOL bShaderLinkStatus = LinkShaderProgramObject(shaderProgramObject);

	if (bShaderLinkStatus == FALSE)
		return FALSE;
	// post link - getting
	projectMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

	lightPosUniform = glGetUniformLocation(shaderProgramObject, "LightPos");
	scaleUniform = glGetUniformLocation(shaderProgramObject, "Scale");

	textureSamplerUniform = glGetUniformLocation(shaderProgramObject, "u_textureSampler");

	// skyColorUniform = glGetUniformLocation(shaderProgramObject, "SkyColor"); // Sky related uniform
	// cloudColorUniform = glGetUniformLocation(shaderProgramObject, "CloudColor"); // Sky related uniform

	// vao and vba related code
	// declartions of vertex Data array
	const GLfloat trianglePosition[] = {
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f};

	const GLfloat normal[] = {
		0.0f, 0.0f, 1.0f, // top-right of front
		0.0f, 0.0f, 1.0f, // top-left of front
		0.0f, 0.0f, 1.0f, // bottom-left of front
		0.0f, 0.0f, 1.0f, // bottom-right of front
	};

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao and vbo related code
	// vao
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// vbo for position
	glGenBuffers(1, &vbo_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Position);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePosition), trianglePosition, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(PVG_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normal
	glGenBuffers(1, &vbo_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Normal);

	// glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(PVG_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PVG_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	// Enabaling the texture
	glEnable(GL_TEXTURE_3D);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	CreateNoise3D(&texture_noise);

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

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (glAlphaEnable)
		glEnable(GL_ALPHA_TEST);
	else
		glDisable(GL_ALPHA_TEST);

	// use shader program obejct
	glUseProgram(shaderProgramObject);

	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, 0.0f, -2.0f); // glTranslatef() is replaced by this line
	rotateMatrix = vmath::rotate(25.0f, -1.0f, 0.0f, 0.0f);

	modelMatrix = translationMatrix * rotateMatrix;

	glUniformMatrix4fv(projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glUniform3fv(lightPosUniform, 1, lightPos);
	glUniform1f(scaleUniform, scaleFactor);

	glUniform3fv(skyColorUniform, 1, skyColor);
	glUniform3fv(cloudColorUniform, 1, cloudColor);

	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_3D, texture_noise);
	glUniform1i(textureSamplerUniform, 0); //

	glBindVertexArray(vao);

	/// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */
	if (isScaled == TRUE)
	{
		scaleFactor = scaleFactor - 0.004f;

		if (scaleFactor < 1.0f)
			isScaled = FALSE;
	}
	else
	{
		// scaleFactor = scaleFactor + 0.1f;
		scaleFactor = scaleFactor + 0.004f;
		if (scaleFactor > 1.5f)
			isScaled = TRUE;
	}
	// skyColor[3] = alphaSky;
	// cloudColor[3] = alphaCloud;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	if (vbo_Normal)
	{
		glDeleteBuffers(1, &vbo_Normal);
		vbo_Normal = 0;
	}

	// deletion of vbo_Color
	if (vbo_Texcoord)
	{
		glDeleteBuffers(1, &vbo_Texcoord);
		vbo_Texcoord = 0;
	}

	// deletion of vbo_Position
	if (vbo_Position)
	{
		glDeleteBuffers(1, &vbo_Position);
		vbo_Position = 0;
	}

	// deletion of vao
	if (vao)
	{
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}

	UninitializeShaders(shaderProgramObject);

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
