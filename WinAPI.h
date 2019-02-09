#pragma once
#include <Windows.h>
#include <vector>
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
class WinAPI
{
	HWND main_window;
	HGLRC gl_context;
	MSG  msg;
	size_t numberLoopFunctions;	
	void(**loopFunctions)(void* argument);
	void** ptrArgument;
public:
	static std::vector<std::wstring> GetNameFolderFiles(std::wstring way);
	void InitWindow();
	void InitOpenGLContext();
	void DispatchMessageWindow();
	void PushFunction(void(*ptrLoopFunctions)(void *argument), void * argument);
	WinAPI();
	~WinAPI();
};

