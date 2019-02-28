#pragma once
#include "clDevice.h"
#include "cpuDevice.h"
#include <time.h>
struct gpu_data {
	size_t width;
	size_t height;
	size_t globalWork[3];
	size_t localWork[3];
	size_t work_size[3];
	clDevice* _device;
	size_t norm_image_gpu_0;
	size_t norm_image_gpu_1;
	size_t memory_buffer;
	cl_uint length_data;
	cl_uint image_indices[1];
	cl_uint buffer_indices[1];
	cl_int kernel_image_gauss;
	cl_int kernel_image_stabilization;
	cl_uint args_gauss_indices[4];
	cl_int length_gauss_args[4];
	cl_uint args_conv_indices[7];
	cl_int length_conv_args[7];
};
struct cpu_data {
	size_t block_x;
	size_t block_y;
	size_t pitch_width_image;
	size_t width_image;
	size_t height_image;
	size_t offset_radius_aligned;
	size_t radius;
	cpuDevice* ptrDevice;
	cl_float4* current_image;
	cl_float4* next_image;
	float pow_sigma;
	float part_block_x;
	float part_block_y;
	float normalize;
};
class Image_Stabilization
{
	gpu_data* _gpu_data;
	cpu_data* _cpu_data;
	void(Image_Stabilization::*ptr_gauss_function)(void* data, void* result);
	void(Image_Stabilization::*ptr_stabilization_function)(void* data, void* result);
public:
	Image_Stabilization(clDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y);
	Image_Stabilization(cpuDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y, cl_uint radius);

	void gpu_Calculate_Gauss_function(void * data, void * result);
	void cpu_sse2_Gauss_function(void * data, void * result);
	void cpu_MSE_SSE2_Stabilization_function(void * data, void * result);
	void gpu_Stabilization_function(void * data_next_image, void * result);
	void Calculate_Gauss_function(void * data, void * result);
	void Stabilization_function(void * data_next_image, void * result);
	void cpu_Stabilization_function(void * data, void * result);
	~Image_Stabilization();
};

