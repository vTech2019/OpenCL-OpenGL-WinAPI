#pragma once
#include "clDevice.h"
#include <fstream>
class fractalImageCompression
{
	void writeImage(cl_uint2* position_xy, cl_float* _q, cl_uchar* typeFigure, size_t size_domain, size_t size_range, char* nameFile, size_t width, size_t height);
	void compress(cl_uint2* errors, cl_uchar* image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height);
	void fractalImageCompression::decompress(cl_uint2* errors, cl_uchar* image, cl_uchar* result_image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height);

	void compress_OpenCL(clDevice* device, cl_uchar* image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height);
public:
	fractalImageCompression(clDevice* device, cl_uchar4* image, size_t width, size_t height, size_t size_domain, size_t size_rank);
	~fractalImageCompression();
};

