#pragma once
#include "clDevice.h"
class MakeNoise
{
	cl_float4* inverse_matrix(cl_float4* matrix, size_t width, size_t height);
	cl_float4* make_kernel_normal_distribution(size_t width, size_t height);
	cl_float4* kernel;
public:
	cl_float4* getKernel();
	MakeNoise(clDevice* device, cl_uchar4* image, size_t width, size_t height, size_t width_filter, size_t height_filter);
	~MakeNoise();
};

