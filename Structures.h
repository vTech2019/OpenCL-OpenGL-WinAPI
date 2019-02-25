#pragma once
#include <stdint.h>
struct GLubyte4 {
	unsigned char r, g, b, a;
};
struct matrix_16 {
	float data[16];
};

struct my_vec3 {
	float x;
	float y;
	float z;
};



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
	bool rgba;
	uint8_t* data;
};