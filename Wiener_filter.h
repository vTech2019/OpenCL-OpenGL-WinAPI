#pragma once
#include "clDevice.h"
class Wiener_filter
{
	clDevice* _device;
	cl_int magnitude_fourier;
	cl_int phase_fourier;
	cl_int convolution_kernel_index;
	cl_int noise_kernel_index;
	cl_int fourier_transform_kernel_index;
	cl_int inverse_fourier_transform_kernel_index;
	cl_int div_fourier_kernel_index;
	cl_int mul_fourier_kernel_index;
	cl_int convert_float4_to_uchar4_kernel_index;
	cl_int fourier_transform_float4_kernel_index;
	cl_int invers_matrix_kernel_index;
	cl_int mul_float4_kernel_index;
	cl_int mean_float4_kernel_index;

	void getSpectrum(size_t real_image_gpu, size_t imagine_image_gpu, size_t result_spectrum, cl_uint width, cl_uint height);

	void getMean(size_t image_gpu, size_t result_image, cl_uint width, cl_uint height);
public:


	Wiener_filter(clDevice* device, cl_uchar4* image, size_t width, size_t height, cl_float4* kernel, size_t width_kernel, size_t height_kernel);
		~Wiener_filter();
};

