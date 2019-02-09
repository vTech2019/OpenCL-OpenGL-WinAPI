#pragma once
//c++17
#include <experimental/filesystem>
#ifdef _WIN32
#include <Windows.h>
#include <gdiplus.h>
#include <Commdlg.h>
#include <immintrin.h>
#include <filesystem>
#pragma comment(lib, "gdiplus.lib")
#endif

struct RGBA {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
};
struct dataImage {
	size_t width;
	size_t height;
	size_t stride;
	int pixelFormat;
	RGBA* data;
};

enum FORMATS { BMP, JPG, GIF, TIF, PNG };

#define _BMP L"{557cf400-1a04-11d3-9a73-0000f81ef32e}"
#define _JPG L"{557cf401-1a04-11d3-9a73-0000f81ef32e}"
#define _GIF L"{557cf402-1a04-11d3-9a73-0000f81ef32e}"
#define _TIF L"{557cf405-1a04-11d3-9a73-0000f81ef32e}"
#define _PNG L"{557cf406-1a04-11d3-9a73-0000f81ef32e}"
void WIN_save_image(dataImage& image, const WCHAR* directory_name, const WCHAR* name, size_t format);

dataImage WIN_load_image(const WCHAR * name);
std::vector<std::wstring> WIN_get_name_folder_files(const wchar_t* name_folder);


