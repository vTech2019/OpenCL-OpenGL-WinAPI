#pragma once
#include "clDevice.h"
class Image_Stabilization
{
	size_t localWork[3];
	size_t work_size[3];
	clDevice* _device;
	cl_uint norm_image_gpu_0;
	cl_uint norm_image_gpu_1;
	cl_uint memory_buffer;
	cl_uint length_data;
	cl_uint image_indices[1];
	cl_uint buffer_indices[1];
	cl_int kernel_image_gauss;
	cl_int kernel_image_stabilization;
	cl_uint args_gauss_indices[4];
	cl_int length_gauss_args[4];
	cl_uint args_conv_indices[7];
	cl_int length_conv_args[7];
public:
	Image_Stabilization(clDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y);
	void Calculate_Gauss_function(void * data, void * result);
	void Stabilization_function(void * data_next_image, void * result, size_t width_current, size_t height_current);
	~Image_Stabilization();
};

