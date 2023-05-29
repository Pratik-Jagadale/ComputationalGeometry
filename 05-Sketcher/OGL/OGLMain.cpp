/* Header Files */
#include <windows.h>
#include "OGLMain.h"
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "Sphere.h"
#include "GLLog.h"

/* OpenGL Header files */
#include "vmath.h"
#include "GLShaders.h"
#include "GLLog.h"
#include "Noise.h"
#include "FontShader.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Line.h"
#include "Point.h"
#include "STLImporter.h"


//#include "UseSkyShader.h"
#include "UseWoodShader.h"
//#include "UseMarbleShader.h"
//#include "UseSunSurfaceShader.h"

//#pragma comment(lib, "Sphere.lib")`

using namespace vmath;

#define WINWIDTH 800
#define WINHEIGHT 600

/* Global Function Declartion */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ToggleFullScreen();
int initialize(void);
BOOL gbActiveWindow = FALSE;

// global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
BOOL gbFullScreen = FALSE;

//CloudShader cloudShader_obj;
WoodShader woodShader_obj;
//MarbleShader marbleShader_obj;
//SunSurfaceShader sunSurfaceShader;

mat4 perspectiveProjectionMatrix;

// Cyilnder
GLuint vao;
GLuint vbo_Position;
GLuint vb_Texcoord;
GLuint vbo_Normal;

vec3 eye = vec3(0.0f, 0.0f, 6.0f);
vec3 center = vec3(0.0f, 0.0f, 0.0f);
vec3 up = vec3(0.0f, 1.0f, 0.0f);

/* MARBLE RELATED VARIABLES */
GLuint texture_noise;
GLuint texture_noise_cloud;

GLfloat scaleFactor = 2.0f;

Line *line = NULL;

enum
{
	SHAPE_LINE,
	SHAPE_CIRCLE,
	SHAPE_CURVE,
	SHAPE_CYLINDER,
	SHAPE_CONE
};

int numberOfShape = SHAPE_LINE;

// FONT
FontShader fontShader;
GLuint VAO_FONT;
GLuint VBO_FONT;

void RenderText(std::string text, float x, float y, float scale, vec3 color);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	ivec2   Size;      // Size of glyph
	ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;

void* ptr;

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
	int initializeFont(void);

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

	fontShader.initialize_cloudShaderObject();

	//status = cloudShader_obj.initialize_cloudShaderObject();

	//status = marbleShader_obj.initialize_marbleShaderObject();

	//status = sunSurfaceShader.initialize_SunSurafaceShaderObject();

	//line = new Line(Point(-1.0, 0.0, 0.0), Point(1.0, 0.0, 0.0));

	// Depth Related Changes
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);

	// Enabaling the texture
	glEnable(GL_TEXTURE_3D);

	/* Clear the screen using blue color */
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	perspectiveProjectionMatrix = mat4::identity();

	CreateNoise3D(&texture_noise);
	CreateNoise3D(&texture_noise_cloud);

	initializeFont();

	// warmup resize call
	resize(WINWIDTH, WINHEIGHT);

	return (0);
}

int initializeFont(void)
{
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		PrintLog("ERROR::FREETYPE: Could not init FreeType Library");
		return -1;
	}

	// find path to font
	// std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");
	std::string font_name = "Antonio-Bold.ttf";
	if (font_name.empty())
	{
		PrintLog("ERROR::FREETYPE: Failed to load font_name" );
		return -1;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		PrintLog("ERROR::FREETYPE: Failed to load font" );
		return -1;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				PrintLog("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAO_FONT);
	glBindVertexArray(VAO_FONT);
	{
		glGenBuffers(1, &VBO_FONT);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_FONT);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	return 0;
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
	//glUseProgram(sunSurfaceShader.shaderProgramObject);

	//// Tranformations
	//mat4 translationMatrix = mat4::identity();
	//mat4 rotateMatrix = mat4::identity();
	//mat4 scaleMatrix = mat4::identity();
	//mat4 modelMatrix = mat4::identity();
	//mat4 viewMatrix = mat4::identity();

	//viewMatrix = vmath::lookat(eye, center, up);
	//translationMatrix = vmath::translate(1.2f, 0.0f, 0.0f);
	//rotateMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	//scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	//modelMatrix = translationMatrix * scaleMatrix;

	//// modelMatrix = translationMatrix * rotateMatrix;

	//glUniformMatrix4fv(sunSurfaceShader.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	//glUniformMatrix4fv(sunSurfaceShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	//glUniformMatrix4fv(sunSurfaceShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	//sunSurfaceShader.update_SunSurfaceObjectUniforms();

	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_3D, texture_noise);
	//glUniform1i(sunSurfaceShader.textureSamplerUniform, 0);
	///*
	//	glBindVertexArray(vao_Cylinder);
	//	glDrawArrays(GL_TRIANGLE_STRIP, 0, cylinder->vertices->size);
	//	glBindVertexArray(0);
	//*/
	//// unuse the shader program object
	//glUseProgram(0);

	RenderText("This is sample text", -40.0f, 40.0f, 0.15f, vec3(0.5, 0.8f, 0.2f));

	SwapBuffers(ghdc);
}

void RenderText( std::string text, float x, float y, float scale, vec3 color)
{
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// activate corresponding render state	
	//shader.use();
	glUseProgram(fontShader.shaderProgramObject);

	mat4 translationMatrix = mat4::identity();
	mat4 rotateMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	viewMatrix = vmath::lookat(eye, center, up);
	translationMatrix = vmath::translate(0.2f, 0.0f, -6.0f);
	rotateMatrix = vmath::rotate(90.0f, 1.0f, 0.0f, 0.0f);
	scaleMatrix = vmath::scale(2.0f, 2.0f, 2.0f);

	modelMatrix = translationMatrix * scaleMatrix;

	// modelMatrix = translationMatrix * rotateMatrix;
	perspectiveProjectionMatrix = vmath::ortho(-100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);

	glUniformMatrix4fv(fontShader.projectMatrixUniform, 1, GL_FALSE, perspectiveProjectionMatrix);
	glUniformMatrix4fv(fontShader.viewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(fontShader.modelMatrixUniform, 1, GL_FALSE, modelMatrix);

	glUniform3f(glGetUniformLocation(fontShader.shaderProgramObject, "textColor"), color[0], color[1], color[2]);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(fontShader.textureSamplerUniform, 0);

	glBindVertexArray(VAO_FONT); 

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing[0] * scale;
		float ypos = y - (ch.Size[1] - ch.Bearing[1]) * scale;

		float w = ch.Size[0] * scale;
		float h = ch.Size[1] * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO_FONT);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

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
	//free(cylinder);

	if (gbFullScreen)
		ToggleFullScreen();

	/*  */
	fontShader.uninitialize();

	UninitializeShaders(woodShader_obj.shaderProgramObject);
	/*UninitializeShaders(cloudShader_obj.shaderProgramObject);
	UninitializeShaders(marbleShader_obj.shaderProgramObject);
	UninitializeShaders(sunSurfaceShader.shaderProgramObject);*/

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
