#include "Wiener_filter.h"
void Wiener_filter::getSpectrum(cl_uint real_image_gpu, cl_uint imagine_image_gpu, cl_uint result_spectrum, cl_uint width, cl_uint height) {
	{
		cl_uint indices[] = { real_image_gpu, imagine_image_gpu, result_spectrum };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		_device->callOpenclFunction(magnitude_fourier, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	{
		cl_uint indices[] = { result_spectrum,  result_spectrum, result_spectrum };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		_device->callOpenclFunction(mul_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
}
void Wiener_filter::getMean(cl_uint image_gpu, cl_uint result_image, cl_uint width, cl_uint height) {
	const void* null_ptr = NULL;
	//cl_float4 sum = { 0.0f, 0.0f, 0.0f, 0.0f };
	//cl_float4* ptr = &sum;
	cl_char k = 0;
	size_t mod_y = sqrt(_device->DeviceInfo.maxWorkGroupSize);
	size_t mod_x = _device->DeviceInfo.maxWorkGroupSize / mod_y;
	size_t _width_memory = width / mod_x;
	size_t _height_memory = width / mod_y;
	size_t length_row_pitch_data = _width_memory * sizeof(cl_float4);
	cl_uint sum_1_result_gpu = _device->mallocImageMemory(null_ptr, _height_memory, _width_memory, length_row_pitch_data, CL_RGBA, CL_FLOAT);
	_width_memory = _width_memory / mod_x == 0 ? 1 : _width_memory / mod_x;
	_height_memory = _height_memory / mod_y == 0 ? 1 : _height_memory / mod_y;
	length_row_pitch_data = _width_memory * sizeof(cl_float4);
	cl_uint sum_2_result_gpu = _device->mallocImageMemory(null_ptr, _height_memory, _width_memory, length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint indices[] = { image_gpu, sum_1_result_gpu, sum_1_result_gpu, sum_2_result_gpu };
	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), -(int)_device->DeviceInfo.maxWorkGroupSize * 16 };
	for (size_t j = width, i = height, firstCall = true; j > 1 && i > 1; j /= mod_x, i /= mod_y) {
		cl_uint indices_args[] = { j, i, firstCall, width * height, NULL };
		_device->callOpenclFunction(mean_float4_kernel_index, indices + k, (cl_char*)indices_args, length_args, 2, 5);
		k ^= 2;
		firstCall = false;
	}
	k = k ^ 2 + 1;
	_device->copy2DImage(indices[k], result_image, 1, 1);
	//_device->readImage((void**)&ptr, &result_image, NULL, &_width_memory, &_height_memory, 1);
	_device->freeImageMemory(sum_2_result_gpu);
	_device->freeImageMemory(sum_1_result_gpu);
}
Wiener_filter::Wiener_filter(clDevice* device, cl_uchar4* image, size_t width, size_t height, cl_float4* kernel, size_t width_kernel, size_t height_kernel)
{
	size_t one_to_one_image = 1;
	const void* null_ptr = NULL;
	cl_uchar type_arguments[] = { sizeof(cl_uint) };
	_device = device;
	size_t length_row_pitch_data[] = { width * sizeof(cl_float4), width * sizeof(cl_uchar4), width_kernel * sizeof(cl_float4), width_kernel * sizeof(cl_uchar4), one_to_one_image * sizeof(cl_float4) };
	magnitude_fourier = device->findKernel((const cl_char*)"fourier_magnitude_float4_image_rgba", sizeof("fourier_magnitude_float4_image_rgba"));
	phase_fourier = device->findKernel((const cl_char*)"fourier_phase_float4_image_rgba", sizeof("fourier_phase_float4_image_rgba"));
	convolution_kernel_index = device->findKernel((const cl_char*)"convolution_f_image_rgba", sizeof("convolution_f_image_rgba"));
	noise_kernel_index = device->findKernel((const cl_char*)"noise_image_rgba", sizeof("noise_image_rgba"));
	fourier_transform_kernel_index = device->findKernel((const cl_char*)"fourier_transform_rgba_image", sizeof("fourier_transform_rgba_image"));
	inverse_fourier_transform_kernel_index = device->findKernel((const cl_char*)"inverse_fourier_transform_rgba_image", sizeof("inverse_fourier_transform_rgba_image"));
	div_fourier_kernel_index = device->findKernel((const cl_char*)"div_fourier_image_rgba_image", sizeof("div_fourier_image_rgba_image"));
	mul_fourier_kernel_index = device->findKernel((const cl_char*)"mul_fourier_image_rgba_image", sizeof("mul_fourier_image_rgba_image"));
	convert_float4_to_uchar4_kernel_index = device->findKernel((const cl_char*)"convert_float4_to_uchar4_image_rgba", sizeof("convert_float4_to_uchar4_image_rgba"));
	fourier_transform_float4_kernel_index = device->findKernel((const cl_char*)"fourier_transform_float_rgba_image", sizeof("fourier_transform_float_rgba_image"));
	invers_matrix_kernel_index = device->findKernel((const cl_char*)"inverse_matrix_Gaussian_filter_float", sizeof("inverse_matrix_Gaussian_filter_float"));
	mul_float4_kernel_index = device->findKernel((const cl_char*)"mul_float4_image_rgba", sizeof("mul_float4_image_rgba"));
	mean_float4_kernel_index = device->findKernel((const cl_char*)"sum_float4_image_rgba", sizeof("sum_float4_image_rgba"));

	cl_uint image_gpu = device->mallocImageMemory((const void*)image, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint result_image_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);

	cl_uint real_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint real_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint spectrum_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint spectrum_kernel_gpu = device->mallocImageMemory(null_ptr, height_kernel, width_kernel, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint kernel_gpu = device->mallocImageMemory((const void*)kernel, height_kernel, width_kernel, length_row_pitch_data[2], CL_RGBA, CL_FLOAT);
	cl_uint copy_kernel_gpu = device->mallocImageMemory(null_ptr, height_kernel, width_kernel, length_row_pitch_data[2], CL_RGBA, CL_FLOAT);

	cl_uint real_result_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_result_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint mean_kernel_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);
	cl_uint mean_image_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);

	{
		cl_uint indices[] = { image_gpu, real_image_gpu, imagine_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(fourier_transform_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	{
		cl_uint indices[] = { kernel_gpu, real_kernel_gpu, imagine_kernel_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(fourier_transform_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	{
		cl_uint indices[] = { real_kernel_gpu, real_kernel_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(mul_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 2, 2);
	}
	{
		cl_uint indices[] = { imagine_kernel_gpu, imagine_kernel_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(mul_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 2, 2);
	}
	getMean(image_gpu, mean_image_gpu, width, height);
	getSpectrum(real_image_gpu, imagine_image_gpu, spectrum_image_gpu, width, height);
	getSpectrum(real_kernel_gpu, imagine_kernel_gpu, spectrum_kernel_gpu, width, height);
	{
		cl_uint indices[] = { real_image_gpu, imagine_image_gpu, result_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(inverse_fourier_transform_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}

	device->readImage((void**)&image, &result_image_gpu, type_arguments, &width, &height, 1);
	device->freeImageMemory(mean_image_gpu);
	device->freeImageMemory(mean_kernel_gpu);
	device->freeImageMemory(imagine_result_gpu);
	device->freeImageMemory(real_result_gpu);
	device->freeImageMemory(copy_kernel_gpu);
	device->freeImageMemory(kernel_gpu);
	device->freeImageMemory(spectrum_kernel_gpu);
	device->freeImageMemory(spectrum_image_gpu);
	device->freeImageMemory(imagine_kernel_gpu);
	device->freeImageMemory(real_kernel_gpu);
	device->freeImageMemory(imagine_image_gpu);
	device->freeImageMemory(real_image_gpu);
	device->freeImageMemory(result_image_gpu);
	device->freeImageMemory(image_gpu);
}

Wiener_filter::~Wiener_filter()
{
}