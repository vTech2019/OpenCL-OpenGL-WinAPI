#include "WinAPI.h"

std::vector<std::wstring> WinAPI::GetNameFolderFiles(std::wstring way) {

	WIN32_FIND_DATA FileData;
	HANDLE hFind;
	std::vector<std::wstring> fileName;
	way.append(L"//*");
	hFind = FindFirstFile(way.data(), &FileData);
	way.pop_back();
	do
	{
		if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			fileName.push_back(way + FileData.cFileName);
		}
	} while (FindNextFile(hFind, &FileData));
	FindClose(hFind);
	return fileName;
}

void WinAPI::InitWindow() {

	HINSTANCE hInstance = GetModuleHandle(NULL);
	
	WNDCLASSW window_class;
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.lpszClassName = L"Window";
	window_class.hInstance = hInstance;
	window_class.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	window_class.lpszMenuName = NULL;
	window_class.lpfnWndProc = WndProc;
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	if (!RegisterClass(&window_class))
	{
		HWND hWnd = GetForegroundWindow();
		MessageBox(hWnd, L"Problem with WNDCLASS!", L"Problem with WNDCLASS!", MB_OK);
	}
	RegisterClassW(&window_class);
	main_window = CreateWindowW(window_class.lpszClassName, L"Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 350, 250, NULL, NULL, hInstance, NULL);
}

void WinAPI::InitOpenGLContext() {

	HDC hdc = GetDC(main_window);
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));     
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.iLayerType = PFD_MAIN_PLANE;
	SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd);

	gl_context = wglCreateContext(hdc);
	wglMakeCurrent(hdc, gl_context);
}

void WinAPI::DispatchMessageWindow() {

	if (!main_window)
		return;
	ShowWindow(main_window, SW_SHOW);
	UpdateWindow(main_window);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void WinAPI::PushFunction(void(*ptrLoopFunctions)(void *argument), void *argument)
{
	numberLoopFunctions++;
	void** memoryPtrFunctions = (void**)realloc(loopFunctions, numberLoopFunctions * sizeof(void(*)(void*)));
	loopFunctions = (void(**)(void *argument))memoryPtrFunctions;
	ptrArgument = (void**)realloc(ptrArgument, numberLoopFunctions * sizeof(void*));
	loopFunctions[numberLoopFunctions - 1] = ptrLoopFunctions;
	ptrArgument[numberLoopFunctions - 1] = argument;
}

WinAPI::WinAPI()
{
}


WinAPI::~WinAPI()
{
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	HDC hDC; 
	PAINTSTRUCT ps; 
	RECT rect; 
	COLORREF colorText = RGB(255, 0, 0); 
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hDC = BeginPaint(hwnd, &ps); 
		//for (size_t i = 0; i < numberLoopFunctions; i++) {
		//	loopFunctions[i](ptrArgument[i]);
		//}
		GetClientRect(hwnd, &rect); 
		SetTextColor(hDC, colorText);
		DrawText(hDC, L"hELLo", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER); 
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd, message, wparam, lparam);
}