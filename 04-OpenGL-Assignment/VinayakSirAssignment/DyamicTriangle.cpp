#include <iostream>
#include <sstream>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"


// Global Variables
const char* APP_TITLE = "Dyanmic Triangle";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = NULL;


// Shaders
const GLchar* vertexShaderSrc = 
    "#version 330 core\n"
	"layout (location = 0) in vec3 pos;"
	"layout (location = 1) in vec3 color;"
	"out vec3 color_out;"
	"void main()"
	"{"
	"color_out = color;"
	"   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
	"}";

const GLchar* fragmentShaderSrc = 
    "#version 330 core\n"
	"out vec4 frag_color;"
	"in vec3 color_out;"
	"void main()"
	"{"
	"   frag_color = vec4(color_out, 1.0f);"
	"}";

// Function prototypes
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();
void toggleColor(void);

float red[] = { 1.0f,0.0f,0.0f };
float blue[] = { 0.0f,0.0f,1.0f };
float green[] = { 0.0f,1.0f,0.0f };

GLfloat colors[] = {
		 1.0f,  0.0f, 0.0f,	// Red
		 0.0f, 1.0f, 0.0f,	// Green 
		0.0f, 0.0f, 1.0f	// Blue
};

// EntryPoint Function
int main()
{
	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	// Set up our quad

	// 1. Set up an array of vertices for a triangle
	//   (What is a vertex?)
	GLfloat vertices[] = {
		 0.0f,  0.5f, 0.0f,	// Top
		 0.5f, -0.5f, 0.0f,	// Right 
	    -0.5f, -0.5f, 0.0f,	// Left
	};

	
	
	GLuint vbo, vao, vbo_color;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);					
	glBindBuffer(GL_ARRAY_BUFFER, vbo);		
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	
	glVertexAttribPointer(
		0,							
		3,							
		GL_FLOAT,					
		GL_FALSE,					
		0,							
		NULL);						
	glEnableVertexAttribArray(0);	


	glGenBuffers(1, &vbo_color);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		NULL);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);


	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}

	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader program linker failure " << infoLog << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);

	while (!glfwWindowShouldClose(gWindow))
	{
		
		showFPS(gWindow);

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);


		glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			NULL);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		
		glfwSwapBuffers(gWindow);
	}

	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

	return 0;
}

void toggleColor(void) {

	for (int i = 0; i <sizeof(colors) / sizeof(colors[0]); i += 3) {
		GLfloat temp = colors[i];       // Store the current red component
		colors[i] = colors[i + 1];      // Assign the green component to the red component
		colors[i + 1] = colors[i + 2];  // Assign the blue component to the green component
		colors[i + 2] = temp;           // Assign the stored red component to the blue component
	}
}

bool initOpenGL()
{
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3

	// Create an OpenGL 3.3 core, forward compatible context window
	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(gWindow);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}

	// Set the required callback functions
	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	// Define the viewport dimensions
	glfw_onFramebufferSize(gWindow, gWindowWidth, gWindowHeight);

	return true;
}

//-----------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
//-----------------------------------------------------------------------------
void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void showFPS(GLFWwindow* window)
{
	static double previousSeconds = 0.0;
	static int frameCount = 0;
	double elapsedSeconds;
	double currentSeconds = glfwGetTime(); // returns number of seconds since GLFW started, as double float

	elapsedSeconds = currentSeconds - previousSeconds;

	
	// Limit text updates to 4 times per second
	if (elapsedSeconds > 0.25)
	{
		previousSeconds = currentSeconds;
		double fps = (double)frameCount / elapsedSeconds;
		double msPerFrame = 1000.0 / fps;

		if (msPerFrame > 0.01) {
			toggleColor();
		}


		// The C++ way of setting the window title
		std::ostringstream outs;
		outs.precision(3);	// decimal places
		outs << std::fixed
			<< APP_TITLE << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << msPerFrame << " (ms)";
		glfwSetWindowTitle(window, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
	}

	frameCount++;
}