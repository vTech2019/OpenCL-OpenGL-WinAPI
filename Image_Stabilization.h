#pragma once
#include "clDevice.h"
class Image_Stabilization
{
	size_t work_size[3];
	clDevice* _device;
	cl_uint norm_image_gpu;
	cl_uint memory_buffer;
	cl_uint length_data;
	cl_uint image_indices[1];
	cl_uint buffer_indices[1];
	cl_uint args_indices[4];
	cl_int kernel_image_gauss;
	cl_int kernel_image_stabilization;
	cl_int length_args[4];
public:
	Image_Stabilization(clDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y);
	void Calculate_Gauss_function(void * data, void * result);
	~Image_Stabilization();
};

