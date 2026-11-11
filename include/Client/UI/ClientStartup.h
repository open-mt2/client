#pragma once

#include <Platform/Platform.h>

class CPythonApplication;

bool RunClientGame(CPythonApplication& app, HINSTANCE hInstance,
				   const char* lpCmdLine);
