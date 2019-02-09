#pragma once
#include <stdio.h>
#define GLEW_STATIC
#include "glew/include/GL/glew.h"
#ifndef _WIN64
#pragma comment(lib, "glew/lib/Release/Win32/glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#else
#pragma comment(lib, "glew/lib/Release/x64/glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#endif

struct matrix_4x4 {
	float data[4][4];
};

struct my_vec3 {
	float x;
	float y;
	float z;
};


class glDevice
{
	my_vec3 Eye;
	my_vec3 Center;
	my_vec3 Up;
	matrix_4x4 projectionMatrix;
	matrix_4x4 matrixView;
	matrix_4x4 matrixRotate;
public:
	glDevice();
	~glDevice();
};

