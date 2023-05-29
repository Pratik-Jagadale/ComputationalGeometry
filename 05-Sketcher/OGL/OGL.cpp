/* Header Files */
#include <windows.h>
#include "OGL.h"
#include <stdio.h>
#include <stdlib.h>

/* OpenGL Header files */
#include <GL/glew.h> // This must be before gl.h
#include <GL/gl.h>
#include "vmath.h"

using namespace vmath;

/* OpenGL libraries */
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")

// for modelLoading
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

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
FILE* gpFile = NULL; // FILE* -> #include<stdio.h>

// PP Related Global Variables
GLuint shaderProgramObject;

enum
{
	PRJ_ATRIBUTE_POSITION = 0,
	PRJ_ATRIBUTE_COLOR,
	PRJ_ATRIBUTE_NORMAL,
	PRJ_ATRIBUTE_TEXTURE0
};

GLuint vao_Cube;		  // Vertex Array Object - Cube
GLuint vbo_Cube_Position; // Vertex Buffer Object - Cube- Position
GLuint vbo_Cube_TexCoord; // Vertex Buffer Object - Cube - Color
GLuint vbo_Pyramid_Normal;

GLuint modelMatrixUniform;
GLuint viewMatrixUniform;
GLuint projectionMatrixUniform;

mat4 perspectiveProjectionMatrix;

GLfloat angleCube = 0.0f;

GLuint texture_stone;
GLuint textureSamplerUniform;

// For Three Light on Sphere
GLuint laUniform[3];		   // light Ambiant
GLuint ldUniform[3];		   // light Diffuse
GLuint lsUniform[3];		   // light Spec
GLuint lighPositionUniform[3]; // light Position

GLuint kaUniform; // material amb
GLuint kdUniform; // mat diff
GLuint ksUniform; // mat specular
GLuint materialShininessUniform;

GLuint lightingEnabledUniform;

BOOL bLight;

struct Light
{
	vmath::vec4 lightAmbiant;
	vmath::vec4 lightDiffuse;
	vmath::vec4 lightSpecular;
	vmath::vec4 lightPositions;
};

struct Light lights[3];

GLfloat materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat meterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat materialShineeness = 124.0f;

GLfloat lightAngleOne = 0.0f;
GLfloat lightAngleTwo = 150.0f;
GLfloat lightAngleZero = 300.0f;

GLfloat anglePyramid = 0.0f;

// Model Loading Related Variables
BOOL loadModel(const char*);
void convertToVec3(std::vector<float>& p_vec_Position, std::vector<float>& p_vec_texCoords);
void filterVertices();

class Face
{
public:
	int* vetrices;
	int* textures;
	int* normals;

	Face(int* v, int* t, int* n)
	{
		this->vetrices = v;
		this->textures = t;
		this->normals = n;
	}
	~Face()
	{
		delete this->vetrices;
		delete this->textures;
		delete this->normals;
	}
};
// Model related

std::vector<float*> vertices;
std::vector<float*> texcoords;
std::vector<float*> normals;
std::vector<Face*> faces;
int face_size = 0;

std::vector<float> vertecesAfterFilter;
std::vector<float> texCoordAfterFilter;
std::vector<float> normalAfterFilter;

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
	if (fopen_s(&gpFile, "Log.txt", "w") != 0) // fopen_s -> #include<stdio.h>
	{
		MessageBox(NULL, TEXT("Creation of Log File Failed..!!! Exiting..."), TEXT("File I/O Error"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File is Successfuly Created");
	}

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
		fprintf(gpFile, "Choose Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -2)
	{
		fprintf(gpFile, "Set Pixel format Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -3)
	{
		fprintf(gpFile, "Crete OpenGl Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -4)
	{
		fprintf(gpFile, "Makeing OpenGL as current Context Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -5)
	{
		fprintf(gpFile, "GLEW Initialization Failed...\n");
		uninitialize();
	}
	else if (iRetVal == -6)
	{
		fprintf(gpFile, " loadGLTexture Failed for texture_stone ...\n");
		uninitialize();
	}
	else if (iRetVal == -7)
	{
		fprintf(gpFile, "loadGLTexture Failed for texture kundali ...\n");
		uninitialize();
	}
	else
	{
		fprintf(gpFile, "Initialize Successfull...\n");
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

		case 'L':
		case 'l':
			if (bLight == FALSE)
			{
				bLight = TRUE;
			}
			else
			{
				bLight = FALSE;
			}
			break;
		case 27:
			if (gpFile)
			{
				fprintf(gpFile, "Log File Successfully Closes");
				fclose(gpFile);
				gpFile = NULL;
			}
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
	void printfGLInfo(void);
	void uninitialize(void);
	BOOL LoadGLTexture(GLuint*, TCHAR[]);

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

	if (!loadModel("Barsatpose.obj"))
	{
		return -6;
	}

	filterVertices();

	// convertToVec3(vertecesAfterFilter, texCoordAfterFilter);

	// Print OpenGL Info
	printfGLInfo();

	// vartex Shader
	const GLchar* vertexShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec4 a_position;"
		"in vec3 a_normal;"
		"uniform vec4 u_lightPosition[3];"
		"uniform mat4 u_modelMatrix;"
		"uniform mat4 u_viewMatrix;"
		"uniform mat4 u_projectionMatrix;"
		"uniform int u_lightingEnabled;"
		"out vec3 transformedNormals;"
		"out vec3 lightDirection[3];"
		"out vec3 viewerVector;"
		"void main(void)"
		"{"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;"
		"mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);"
		"transformedNormals = normalize(normalMatrix * a_normal);"
		"viewerVector = normalize(-eyeCoordinates.xyz);"
		"for(int i = 0 ; i < 3 ; i++)"
		"{"
		"lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" // Swizaling
		"}"
		"}"
		"gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;"
		"}";

	GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	glCompileShader(vertexShaderObject);

	GLint status;
	GLint infoLogLength;
	char* log = NULL;
	glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(vertexShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "VERTEX SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// fragment Shader

	status = 0;
	infoLogLength = 0;

	const GLchar* fragmentShaderSourceCode =
		"#version 460 core"
		"\n"
		"in vec3 transformedNormals;"
		"in vec3 lightDirection[3];"
		"in vec3 viewerVector;"
		"uniform vec3 u_la[3];"
		"uniform vec3 u_ld[3];"
		"uniform vec3 u_ls[3];"
		"uniform vec4 u_lightPosition[3];"
		"uniform vec3 u_ka;"
		"uniform vec3 u_ks;"
		"uniform vec3 u_kd;"
		"uniform float u_materialShininnes;"
		"uniform int u_lightingEnabled;"
		"out vec4 FragColor;"
		"vec3 phong_ads_light;"
		"void main(void)"
		"{"
		"phong_ads_light = vec3(0.0,0.0,0.0);"
		"if(u_lightingEnabled == 1)"
		"{"
		"vec3 ambiant[3];"
		"vec3 diffuse[3];"
		"vec3 reflectionVector[3];"
		"vec3 specular[3];"
		"for(int i = 0 ; i < 3 ; i++)"
		"{"
		"ambiant[i] = u_la[i] * u_ka;"
		"diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);"
		"reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);"
		"specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);"
		"phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];"
		"}"
		"}"
		"else"
		"{"
		"phong_ads_light = vec3(1.0,1.0,1.0);"
		"}"
		"FragColor = vec4(phong_ads_light, 1.0);"
		"}";

	GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);

	glCompileShader(fragmentShaderObject);

	glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(fragmentShaderObject, infoLogLength, &written, log);
				fprintf(gpFile, "FRAGMENT SHADER COMPILATION LOG : %s\n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// Shader Program Object
	// pr
	shaderProgramObject = glCreateProgram();
	glAttachShader(shaderProgramObject, vertexShaderObject);

	glAttachShader(shaderProgramObject, fragmentShaderObject);

	// prelinked binding
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_POSITION, "a_position");
	glBindAttribLocation(shaderProgramObject, PRJ_ATRIBUTE_NORMAL, "a_normal");

	// link
	glLinkProgram(shaderProgramObject);

	status = 0;
	infoLogLength = 0;

	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);

	if (status == GL_FALSE)
	{
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char*)malloc(infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				fprintf(gpFile, "SHADER PROGRAM LINK LOG: %s \n", log);
				free(log);
				log = NULL;
				uninitialize();
			}
		}
	}

	// post link - getting
	modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_modelMatrix");
	viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
	projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");

	laUniform[0] = glGetUniformLocation(shaderProgramObject, "u_la[0]");
	ldUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ld[0]");
	lsUniform[0] = glGetUniformLocation(shaderProgramObject, "u_ls[0]");
	lighPositionUniform[0] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[0]");

	laUniform[1] = glGetUniformLocation(shaderProgramObject, "u_la[1]");
	ldUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ld[1]");
	lsUniform[1] = glGetUniformLocation(shaderProgramObject, "u_ls[1]");
	lighPositionUniform[1] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[1]");

	laUniform[2] = glGetUniformLocation(shaderProgramObject, "u_la[2]");
	ldUniform[2] = glGetUniformLocation(shaderProgramObject, "u_ld[2]");
	lsUniform[2] = glGetUniformLocation(shaderProgramObject, "u_ls[2]");
	lighPositionUniform[2] = glGetUniformLocation(shaderProgramObject, "u_lightPosition[2]");

	kaUniform = glGetUniformLocation(shaderProgramObject, "u_ka");
	kdUniform = glGetUniformLocation(shaderProgramObject, "u_kd");
	ksUniform = glGetUniformLocation(shaderProgramObject, "u_ks");
	materialShininessUniform = glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

	lightingEnabledUniform = glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
	// vao and vbo related code

	for (int i = 0; i < vertecesAfterFilter.size(); i++)
		fprintf(gpFile, " %f\n", vertecesAfterFilter[i]);

	fprintf(gpFile, " vertecesAfterFilter.size() = %d\n", vertecesAfterFilter.size());

	// vao for Cube
	glGenVertexArrays(1, &vao_Cube);
	glBindVertexArray(vao_Cube);

	// vbo for position
	glGenBuffers(1, &vbo_Cube_Position);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_Position);
	glBufferData(GL_ARRAY_BUFFER, vertecesAfterFilter.size() * sizeof(float), &vertecesAfterFilter.at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_POSITION);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Texcoord
	glGenBuffers(1, &vbo_Cube_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Cube_TexCoord);
	glBufferData(GL_ARRAY_BUFFER, texCoordAfterFilter.size() * sizeof(float), &texCoordAfterFilter.at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_TEXTURE0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// vbo for Normals
	glGenBuffers(1, &vbo_Pyramid_Normal);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_Pyramid_Normal);
	glBufferData(GL_ARRAY_BUFFER, normalAfterFilter.size() * sizeof(float), &normalAfterFilter.at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(PRJ_ATRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(PRJ_ATRIBUTE_NORMAL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // ubind vao for Cube

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	/* Clear the  screen using blue color */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Texture Initiallization
	if (LoadGLTexture(&texture_stone, MAKEINTRESOURCE(IDBITMAP_STONE)) == FALSE)
		return -6;

	// Enabaling the texture
	glEnable(GL_TEXTURE_2D);

	lights[0].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightDiffuse = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightSpecular = vmath::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	lights[0].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	lights[2].lightAmbiant = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	lights[2].lightDiffuse = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[2].lightSpecular = vmath::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	lights[2].lightPositions = vmath::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);

	return (0);
}

void printfGLInfo(void)
{
	// Local Variable declarations
	GLint numExtensions = 0;

	// Code
	fprintf(gpFile, "OpenGL Vendor: %s\n", glGetString(GL_VENDOR));					 // Graphic Card Compony
	fprintf(gpFile, "OpenGL Renderer: %s\n", glGetString(GL_RENDERER));				 // Graphic Card
	fprintf(gpFile, "OpenGL Version: %s\n", glGetString(GL_VERSION));				 // OpenGL Version
	fprintf(gpFile, "GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); // Graphic Library Shading Language Version

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

	fprintf(gpFile, "Number of Supported Extensions: %d\n", numExtensions);

	for (int i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}
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
	glUseProgram(shaderProgramObject);

	// Pyramid
	// Tranformations
	mat4 translationMatrix = mat4::identity();
	mat4 rotationMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 modelViewProjectionMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	translationMatrix = vmath::translate(0.0f, -1.0f, -4.0f); // glTranslatef() is replaced by this line
	rotationMatrix = vmath::rotate(angleCube, 0.0f, 1.0f, 0.0f);

	// scaleMatrix = vmath::scale(0.25f, 0.25f, 0.25f);
	scaleMatrix = vmath::scale(0.5f, 0.5f, 0.5f);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);

	// Light Related Code
	// Set Light Zero Position - rotating zero light - x Around
	float angle = lightAngleZero * (M_PI / 180.0f);
	float x = 5.0f * cos(angle);
	float y = 5.0f * sin(angle);
	lights[0].lightPositions[1] = x;
	lights[0].lightPositions[2] = y;

	// Set Light One Position  rotating One Light - Y Rotation
	angle = (lightAngleOne * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[1].lightPositions[0] = x;
	lights[1].lightPositions[2] = y;

	// Set Light Two Position rotating Two Light Z Rotation
	angle = (lightAngleTwo * M_PI) / 180.0f;
	x = 5.0f * cos(angle);
	y = 5.0f * sin(angle);
	lights[2].lightPositions[0] = x;
	lights[2].lightPositions[1] = y;

	if (bLight == TRUE)
	{
		glUniform1i(lightingEnabledUniform, 1);

		for (int i = 0; i < 3; i++)
		{
			glUniform3fv(laUniform[i], 1, lights[i].lightAmbiant); // we can use glUniform3f() ,so we can directly pass the values to uniform
			glUniform3fv(ldUniform[i], 1, lights[i].lightDiffuse);
			glUniform3fv(lsUniform[i], 1, lights[i].lightSpecular);
			glUniform4fv(lighPositionUniform[i], 1, lights[i].lightPositions);
		}

		glUniform3fv(kaUniform, 1, materialAmbiant);
		glUniform3fv(kdUniform, 1, meterialDiffuse);
		glUniform3fv(ksUniform, 1, materialSpecular);
		glUniform1f(materialShininessUniform, materialShineeness);
	}
	else
	{
		glUniform1i(lightingEnabledUniform, 0);
	}

	glActiveTexture(GL_TEXTURE0); //
	glBindTexture(GL_TEXTURE_2D, texture_stone);
	glUniform1i(textureSamplerUniform, 0);

	glBindVertexArray(vao_Cube);

	glDrawArrays(GL_TRIANGLES, 0, vertecesAfterFilter.size());

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// unuse the shader program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void update(void)
{
	/* code */

	angleCube = angleCube + 1.0f;
	if (angleCube >= 360.0f)
		angleCube = angleCube - 360.0f;

	lightAngleZero = lightAngleZero + 1.0f;
	if (lightAngleZero > 360.0f)
		lightAngleZero = lightAngleZero - 360.0f;

	lightAngleOne = lightAngleOne + 1.0f;
	if (lightAngleOne > 360.0f)
		lightAngleOne = lightAngleOne - 360.0f;

	lightAngleTwo = lightAngleTwo + 1.0f;
	if (lightAngleTwo > 360.0f)
		lightAngleTwo = lightAngleTwo - 360.0f;
}

void uninitialize(void)
{
	/* function declarations */
	void ToggleFullScreen(void);

	/* code */
	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	// deletion of vbo_Pyramid_Position
	if (vbo_Pyramid_Normal)
	{
		glDeleteBuffers(1, &vbo_Pyramid_Normal);
		vbo_Pyramid_Normal = 0;
	}

	if (texture_stone)
	{
		glDeleteTextures(1, &texture_stone);
		texture_stone = 0;
	}

	if (vbo_Cube_TexCoord)
	{
		glDeleteBuffers(1, &vbo_Cube_TexCoord);
		vbo_Cube_TexCoord = 0;
	}

	// delete vbo_Cube_Position
	if (vbo_Cube_Position)
	{
		glDeleteBuffers(1, &vbo_Cube_Position);
		vbo_Cube_Position = 0;
	}

	// deletion of vao_Cube
	if (vao_Cube)
	{
		glDeleteVertexArrays(1, &vao_Cube);
		vao_Cube = 0;
	}

	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLsizei numAttachedShaders;

		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint* shaderObject = NULL;
		shaderObject = (GLuint*)malloc(numAttachedShaders * sizeof(GLuint));
		glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObject);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObject[i]);
			glDeleteShader(shaderObject[i]);
			shaderObject[i] = 0;
		}
		free(shaderObject);
		shaderObject = NULL;
		glUseProgram(0);
		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}

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

	if (gpFile)
	{
		fprintf(gpFile, "Log File Successfully Closes");
		fclose(gpFile);
		gpFile = NULL;
	}
}

BOOL LoadGLTexture(GLuint* texture, TCHAR ImageResourceID[])
{
	// variable declartions
	HBITMAP hBitmap = NULL;
	BITMAP bmp;
	BOOL bResult = FALSE;

	// code
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), ImageResourceID, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	if (hBitmap)
	{
		bResult = TRUE;
		GetObject(hBitmap, sizeof(BITMAP), &bmp);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // 4 TO 1 FOR BETTER PERFORMANCE

		glGenTextures(1, texture);

		glBindTexture(GL_TEXTURE_2D, *texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// create the texture
		glTexImage2D(GL_TEXTURE_2D,	   // Targter
			0,				   // MipMap Level (done by opengl)
			GL_RGBA,		   // Opengl Image format
			bmp.bmWidth,	   // Image Width
			bmp.bmHeight,	   // Image Height
			0,				   // Border Width
			GL_BGR,		   // Image Format
			GL_UNSIGNED_BYTE, // Data type of bmp.bmBits
			bmp.bmBits);	   //

		glGenerateMipmap(GL_TEXTURE_2D); // Generate MipMap

		glBindTexture(GL_TEXTURE_2D, 0); // unbind texture

		// DELETE Object
		DeleteObject(hBitmap);
	}
	return bResult;
}

BOOL loadModel(const char* fileName)
{

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

	float a, b, c;
	for (int i = 0; i < lines.size(); i++)
	{
		line = lines[i];
		if (line[0] == 'v')
		{
			if (line[1] == ' ')
			{
				sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
				vertices.push_back(new float[3]{ a, b, c });
			}
			else if (line[1] == 't')
			{
				sscanf(line.c_str(), "vt %f %f", &a, &b);
				texcoords.push_back(new float[2]{ a, b });
			}
			else if (line[1] == 'n')
			{
				sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
				normals.push_back(new float[3]{ a, b, c });
			}
		}
		else if (line[0] == 'f')
		{
			int v0, v1, v2, v3, t0, t1, t2, t3, n0, n1, n2, n3;
			sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d ", &v0, &t0, &n0, &v1, &t1, &n1, &v2, &t2, &n2);

			int* v = new int[3]{ v0 - 1, v1 - 1, v2 - 1 };
			int* t = new int[3]{ t0 - 1, t1 - 1, t2 - 1 };
			int* n = new int[3]{ n0 - 1, n1 - 1, n2 - 1 };

			faces.push_back(new Face(v, t, n));
		}
	}
	lines.clear();
	face_size = faces.size();
	return TRUE;
}

void filterVertices()
{

	for (int i = 0; i < face_size; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			int vi = faces.at(i)->vetrices[j];
			int ti = faces.at(i)->textures[j];
			int ni = faces.at(i)->normals[j];

			texCoordAfterFilter.push_back(texcoords[ti][0]);
			texCoordAfterFilter.push_back(texcoords[ti][1]);

			vertecesAfterFilter.push_back(vertices[vi][0]);
			vertecesAfterFilter.push_back(vertices[vi][1]);
			vertecesAfterFilter.push_back(vertices[vi][2]);

			normalAfterFilter.push_back(normals[ni][0]);
			normalAfterFilter.push_back(normals[ni][1]);
			normalAfterFilter.push_back(normals[ni][2]);
			// vertecesAfterFilter.push_back(vertices[vi][3]);

			// fprintf(gfFile_olp, "%f %f %f \n", vertices[vi][0], vertices[vi][1], vertices[vi][2]);
		}
	}
}

vector<float> vec_normal_stl;
vector<float> vec_vertices_stl;

BOOL loadSTL(const char* fileName)
{

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

	float a, b, c;
	for (int i = 0; i < lines.size(); i++)
	{
		line = lines[i];
		//if (line[0] == 'v')
		//{
		if (line == "facet normal")
		{
			sscanf(line.c_str(), "facet normal %f %f %f", &a, &b, &c);
			//vertices.push_back(new float[3]{ a, b, c });
			for (int i = 0; i < 3; i++) // for per vertex normal
			{
				vec_normal_stl.push_back(a);
				vec_normal_stl.push_back(b);
				vec_normal_stl.push_back(c);
			}

		}
		else if (line == "        vertex")
		{
			sscanf(line.c_str(), "        vertex %f %f %f", &a, &b, &c);
			//texcoords.push_back(new float[2]{ a, b });
			vec_vertices_stl.push_back(a);
			vec_vertices_stl.push_back(b);
			vec_vertices_stl.push_back(c);
		}
		//}

	}
	lines.clear();

	return TRUE;
}
