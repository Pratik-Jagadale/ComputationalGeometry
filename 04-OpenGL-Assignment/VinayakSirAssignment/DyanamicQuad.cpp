#include <iostream>
#include <sstream>
#include<vector>

#define GLEW_STATIC
#include "GL/glew.h"	
#include "GLFW/glfw3.h"

const char* APP_TITLE = "Dyamic Quad On Mouse Click";
const int gWindowWidth = 800;
const int gWindowHeight = 600;
GLFWwindow* gWindow = NULL;
std::vector<float> squareVertices;
bool mouseButtonDown = false;
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void glfw_onFramebufferSize(GLFWwindow* window, int width, int height);
void showFPS(GLFWwindow* window);
bool initOpenGL();

// Shaders
const GLchar* vertexShaderSrc =
"#version 330 core\n"
"layout (location = 0) in vec3 pos;"
"layout (location = 1) in vec3 color;"
"\n"
"out vec3 vert_color;"
"void main()"
"{"
"    vert_color = color;"
"    gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);"
"}";

const GLchar* fragmentShaderSrc =
"#version 330 core\n"
"in vec3 vert_color;"
"out vec4 frag_color;"
"void main()"
"{"
"    frag_color = vec4(1.0,1.0,1.0, 1.0);"
"}";

int main()
{

	if (!initOpenGL())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return -1;
	}

	GLfloat vert_square[] = {
		1.0f, 0.0f, 0.0f,  // vertex 0 color
		0.0f, 1.0f, 0.0f,  // vertex 1 color
		0.0f, 0.0f, 1.0f,  // vertex 2 color
	};

	// 2. Set up buffers on the GPU
	GLuint vbo, vbo2, vao;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, squareVertices.size() * sizeof(float), NULL, GL_DYNAMIC_DRAW);

	/*glGenBuffers(1, &vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vert_square), vert_square, GL_DYNAMIC_DRAW);*/


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	// Now bind the color VBO and set the vertix attrib pointer
	//glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(1);

	glEnableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_COLOR_ARRAY);

	// 3. Create vertex shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertexShaderSrc, NULL);
	glCompileShader(vs);

	//glColorPointer(3, GL_FLOAT, 0, vert_square);

	// Check for compile errors
	GLint result;
	GLchar infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Vertex shader failed to compile. " << infoLog << std::endl;
	}

	// 4. Create fragment shader
	GLint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fs);

	// Check for compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fs, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Fragment shader failed to compile. " << infoLog << std::endl;
	}

	// 5. Create shader program and link shaders to program
	GLint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vs);
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);

	// Check for linker errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
		std::cout << "Error! Shader program linker failure. " << infoLog << std::endl;
	}

	// Clean up shaders, do not need them anymore since they are linked to a shader program
	glDeleteShader(vs);
	glDeleteShader(fs);


	// Rendering loop
	while (!glfwWindowShouldClose(gWindow))
	{
		showFPS(gWindow);

		// Poll for and process events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the quad (two triangles
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);


		/*glBindBuffer(GL_ARRAY_BUFFER, vbo2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vert_square), vert_square, GL_DYNAMIC_DRAW);*/


		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, squareVertices.size() * sizeof(float), squareVertices.data(), GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);


		glDrawArrays(GL_LINES, 0, squareVertices.size() / 3);

		glBindVertexArray(0);
		// Swap front and back buffers
		glfwSwapBuffers(gWindow);
	}

	// Clean up
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

	return 0;
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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);
	if (gWindow == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}


	glfwMakeContextCurrent(gWindow);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return false;
	}


	glfwSetKeyCallback(gWindow, glfw_onKey);
	glfwSetFramebufferSizeCallback(gWindow, glfw_onFramebufferSize);
	glfwSetMouseButtonCallback(gWindow, mouseButtonCallback);

	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	glfw_onFramebufferSize(gWindow, gWindowWidth, gWindowHeight);


	return true;
}
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			mouseButtonDown = true;

			double mouseX, mouseY;
			glfwGetCursorPos(gWindow, &mouseX, &mouseY);

			float x = (mouseX / gWindowWidth) * 2 - 1;
			float y = 1 - (mouseY / gWindowHeight) * 2;

			if (squareVertices.size() < 22) {

				if (squareVertices.size() != 0)
				{
					squareVertices.push_back(x);
					squareVertices.push_back(y);
					squareVertices.push_back(0.0f);
				}

				squareVertices.push_back(x);
				squareVertices.push_back(y);
				squareVertices.push_back(0.0f);

				if (squareVertices.size() > 18) {
					squareVertices.push_back(squareVertices[0]);
					squareVertices.push_back(squareVertices[1]);
					squareVertices.push_back(squareVertices[2]);
				}
			}
		}
		if (action == GLFW_RELEASE)
		{
			mouseButtonDown = false;
		}
	}
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

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void glfw_onFramebufferSize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}