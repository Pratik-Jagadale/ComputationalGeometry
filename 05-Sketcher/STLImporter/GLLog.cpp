#include "pch.h"
#include "GLLog.h"

//global variable declarations
FILE *gpFile = NULL;

BOOL CreateLogFile(void)
{
	//code
	if (gpFile != NULL)
		return(FALSE);

	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Failed To Create Log File \"Log.txt\" !!! Exitting Now ..."), TEXT("LOG FILE ERROR"), MB_ICONERROR | MB_OK);
		return(FALSE);
	}

	else
	{
		fprintf(gpFile, "============================================================\n");
		fprintf(gpFile, "Log File \"Log.txt\" Has Been Created Successfully !!!\n");
		fprintf(gpFile, "============================================================\n\n");
	}
	return(TRUE);
}

void PrintGLInfo(void)
{
	// variable declarations
	GLint numExtensions, i;

	// code
	fprintf(gpFile, "\n");
	fprintf(gpFile, "OpenGL Related Information : \n");
	fprintf(gpFile, "===============================\n\n");
	fprintf(gpFile, "OpenGL Vendor   : %s\n", glGetString(GL_VENDOR));
	fprintf(gpFile, "OpenGL Renderer : %s\n", glGetString(GL_RENDERER));
	fprintf(gpFile, "OpenGL Version  : %s\n", glGetString(GL_VERSION));
	fprintf(gpFile, "GLSL Version    : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
	fprintf(gpFile, "Number Of Supportrd Extensions = %d\n", numExtensions);

	for (i = 0; i < numExtensions; i++)
	{
		fprintf(gpFile, "%s\n", glGetStringi(GL_EXTENSIONS, i));
	}

	fprintf(gpFile, "===============================\n\n");
}

void PrintLog(const char *fmt, ...)
{
	//variable declarations
	va_list arg;
	int ret;

	//code
	if (gpFile == NULL)
		return;
	va_start(arg, fmt);
	ret = vfprintf(gpFile, fmt, arg);
	va_end(arg);
}

void CloseLogFile(void)
{
	//code
	if (gpFile == NULL)
		return;

	fprintf(gpFile, "============================================================\n");
	fprintf(gpFile, "Log File \"Log.txt\" Has Been Closed Successfully !!!\n");
	fprintf(gpFile, "============================================================\n");

	fclose(gpFile);
	gpFile = NULL;
}

