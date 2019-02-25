#pragma once
#include <Windows.h>
#include "glDevice.h"
#include <vector>
class WinAPI
{
	HWND main_window;
	HGLRC gl_context;
	MSG  msg;
	static size_t numberLoopFunctions;
	static void(**loopFunctions)(void* argument);
	static void** ptrArgument;
public:
	void pushFunction(void(*ptrLoopFunctions)(void *argument), void * argument);
	static std::vector<std::wstring> GetNameFolderFiles(std::wstring way);
	void InitWindow(wchar_t * name_window);
	void InitOpenGLContext();
	void DispatchMessageWindow();
	WinAPI();
	~WinAPI(); 
	void pushFunction(void(glDevice::*ptrLoopFunctions)(void *argument), void *argument);

	void PushFunction(void(*ptrLoopFunctions)(void *argument), void * argument);
	static LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
};

