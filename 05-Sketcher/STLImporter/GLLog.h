#pragma once
#include "pch.h"

#include <windows.h> // For BOOL, TEXT(), MessageBox(), etc.
#include <stdio.h> // For fopen_s(), fprintf(), fclose()

#include "GLHeadersAndMacros.h"

BOOL CreateLogFile(void);
void PrintGLInfo(void);
void PrintLog(const char *, ...);
void CloseLogFile(void);

