#include <io.h>

#include "GLShaders.h"

GLuint CreateAndCompileShaderObjects(const char *shaderFilename, GLShaderType shaderType)
{
	// variable declarations
	GLuint shaderObject = 0;
	char shaderNameTag[50];
	GLenum glShaderType;

	int fdShaderFile = -1;
	long shaderFileSize = -1;
	char *shaderSourceCodeBuffer = NULL;

	int status = 0;
	int infoLogLength = 0;
	char *log = NULL;

	// code
	fdShaderFile = _open(shaderFilename, _O_RDONLY, _S_IREAD);
	if (fdShaderFile == -1)
	{
		PrintLog("Failed To Open Shader File %s! Exitting Now ...\n", shaderFilename);
		return(0);
	}

	shaderFileSize = _lseek(fdShaderFile, 0, SEEK_END);
	shaderFileSize = shaderFileSize + 1; // '\0'
	shaderSourceCodeBuffer = (char *)malloc(shaderFileSize);
	_lseek(fdShaderFile, 0, SEEK_SET);

	shaderFileSize = _read(fdShaderFile, (char *)shaderSourceCodeBuffer, shaderFileSize);
	if (shaderFileSize < 0)
	{
		PrintLog("Failed To Read Shader File %s! Exitting Now ...\n", shaderFilename);
		return(0);
	}
	
	shaderSourceCodeBuffer[shaderFileSize] = '\0';
	_close(fdShaderFile);

	switch (shaderType)
	{
	case VERTEX:
		strcpy_s(shaderNameTag, 50, "Vertex");
		glShaderType = GL_VERTEX_SHADER;
		break;

	case TESSELLATION_CONTROL:
		strcpy_s(shaderNameTag, 50, "Tessellation Control");
		glShaderType = GL_TESS_CONTROL_SHADER;
		break;

	case TESSELLATION_EVALUATION:
		strcpy_s(shaderNameTag, 50, "Tessellation Evaluation");
		glShaderType = GL_TESS_EVALUATION_SHADER;
		break;

	case GEOMETRY:
		strcpy_s(shaderNameTag, 50, "Geometry");
		glShaderType = GL_GEOMETRY_SHADER;
		break;

	case FRAGMENT:
		strcpy_s(shaderNameTag, 50, "Fragment");
		glShaderType = GL_FRAGMENT_SHADER;
		break;

	case COMPUTE:
		strcpy_s(shaderNameTag, 50, "Compute");
		glShaderType = GL_COMPUTE_SHADER;
		break;

	default:
		PrintLog("Invalid Shader Type! Exitting Now ...\n");
		return(0);
	}

	// CREATING SHADER OBJECT
	shaderObject = glCreateShader(glShaderType);
	if (shaderObject == 0)
	{
		PrintLog("\"%s\" : Failed To Create %s Shader Object! Exitting Now ...\n", shaderFilename, shaderNameTag);
		return(0);
	}

	glShaderSource(shaderObject, 1, (const GLchar **)&shaderSourceCodeBuffer, NULL);
	free(shaderSourceCodeBuffer);

	glCompileShader(shaderObject);

	// Step 5

	// 5(a)
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		PrintLog("\"%s\" : %s Shader Compilation Failed.\n", shaderFilename, shaderNameTag);
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(sizeof(char) * infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(shaderObject, infoLogLength, &written, log);
				PrintLog("\"%s\" : %s Shader Compilation Log : %s\n", shaderFilename, shaderNameTag, log);
				free(log);
				return(0);
			}
		}
	}

	else
	{
		PrintLog("\"%s\" : %s Shader Compilation Succeeded.\n", shaderFilename, shaderNameTag);
	}

	return(shaderObject);
}

BOOL LinkShaderProgramObject(GLuint shaderProgramObject)
{
	// variable declarations
	int status = 0;
	int infoLogLength = 0;
	char *log = NULL;

	// code
	glLinkProgram(shaderProgramObject);

	// Step 4
	glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		PrintLog("Shader Program Linking Failed.\n");
		glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0)
		{
			log = (char *)malloc(sizeof(char) * infoLogLength);
			if (log != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(shaderProgramObject, infoLogLength, &written, log);
				PrintLog("Shader Program Linking Log : %s\n", log);
				free(log);
				return(FALSE);
			}
		}
	}

	else
	{
		PrintLog("Shader Program Linking Succeeded.\n");
	}

	return(TRUE);
}

void UninitializeShaders(GLuint shaderProgramObject)
{
	// code
	if (shaderProgramObject)
	{
		glUseProgram(shaderProgramObject);

		GLsizei numAttachedShaders = 0;
		glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numAttachedShaders);

		GLuint *shaderObjects = NULL;
		shaderObjects = (GLuint *)malloc(sizeof(GLuint) * numAttachedShaders);

		glGetAttachedShaders(shaderProgramObject, numAttachedShaders, &numAttachedShaders, shaderObjects);

		for (GLsizei i = 0; i < numAttachedShaders; i++)
		{
			glDetachShader(shaderProgramObject, shaderObjects[i]);
			glDeleteShader(shaderObjects[i]);
			shaderObjects[i] = 0;
		}

		free(shaderObjects);
		shaderObjects = NULL;

		glUseProgram(0);

		glDeleteProgram(shaderProgramObject);
		shaderProgramObject = 0;
	}
}

