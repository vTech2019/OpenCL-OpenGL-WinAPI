#pragma once
#include "../clDevice.h"
#include "../cpuDevice.h"
extern "C"{
 void	_fastcall _cpu_sse2_Gauss_function(void* data, void* result, uint32_t width, uint32_t height, uint16_t part_block_x, uint16_t part_block_y, float pow_sigma, uint32_t pitch_width, uint32_t aligned_offset, uint32_t block_x, uint32_t block_y);
};
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
	uint32_t block_x;
	uint32_t block_y;
	uint32_t pitch_width_image;
	uint32_t width_image;
	uint32_t height_image;
	uint32_t offset_radius_aligned;
	uint32_t radius;
	cpuDevice* ptrDevice;
	cl_float4* current_image;
	cl_float4* next_image;
	float pow_sigma;
	uint16_t part_block_x;
	uint16_t part_block_y;
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

