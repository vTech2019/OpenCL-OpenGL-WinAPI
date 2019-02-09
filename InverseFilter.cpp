#include "InverseFilter.h"
void InverseFilter::getSpectrum(cl_uint real_image_gpu, cl_uint imagine_image_gpu, cl_uint result_spectrum, cl_uint width, cl_uint height) {
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
void InverseFilter::getMeanOrMax(cl_uint image_gpu, cl_uint result_image, cl_uint width, cl_uint height, bool mean) {
	const void* null_ptr = NULL;
	size_t _type_image[] = { CL_RGBA };
	size_t _type_data[] = { CL_FLOAT };
	cl_float4 sum = { 0.0f, 0.0f, 0.0f, 0.0f };
	cl_float4* ptr = &sum;
	cl_char k = 0;
	size_t mod_y = sqrt(_device->DeviceInfo.maxWorkGroupSize);
	size_t mod_x = _device->DeviceInfo.maxWorkGroupSize / mod_y;
	size_t _width_memory = width / mod_x + 1;
	size_t _height_memory = width / mod_y + 1;
	size_t length_row_pitch_data = _width_memory * sizeof(cl_float4);
	cl_uint sum_1_result_gpu = _device->mallocImageMemory(&null_ptr, _height_memory, _width_memory, length_row_pitch_data, CL_RGBA, CL_FLOAT);
	_width_memory = _width_memory / mod_x == 0 ? 1 : _width_memory / mod_x;
	_height_memory = _height_memory / mod_y == 0 ? 1 : _height_memory / mod_y;
	length_row_pitch_data = _width_memory * sizeof(cl_float4);
	cl_uint sum_2_result_gpu = _device->mallocImageMemory(&null_ptr, _height_memory, _width_memory, length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint indices[] = { image_gpu, sum_1_result_gpu, sum_1_result_gpu, sum_2_result_gpu };
	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), -(int)_device->DeviceInfo.maxWorkGroupSize * 16 };
	for (size_t j = width, i = height, firstCall = true; j > 1 && i > 1; j /= mod_x, i /= mod_y) {
		cl_uint indices_args[] = { j, i, firstCall, width * height, mean, NULL };
		_device->callOpenclFunction(mean_float4_kernel_index, indices + k, (cl_char*)indices_args, length_args, 2, 6);
		k ^= 2;
		firstCall = false;
	}
	k = k ^ 2 + 1;
	_device->copy2DImage(indices[k], result_image, 1, 1);
	_device->readImage((void**)&ptr, &result_image, NULL, &_width_memory, &_height_memory, 1);
	_device->freeImageMemory(sum_1_result_gpu);
	_device->freeImageMemory(sum_2_result_gpu);
	cl_float4* data = (cl_float4*)malloc(width*height * sizeof(cl_float4));
	_width_memory = width;
	_height_memory = height;
	_device->readImage((void**)&data, &image_gpu, NULL, &_width_memory, &_height_memory, 1);
	float summm = 0;
	for (size_t i = 0; i < height*width; i++)
		summm += data[i].x;
	printf("mean = %f\n", sum.x);
	printf("sum = %f\n", summm);
	free(data);
}
//void Lab_2::convolution_rgba_host_image(size_t index_kernel, clDevice* device, cl_uchar4* image, size_t width, size_t height, cl_float4* kernel, size_t width_kernel, size_t height_kernel) {
//	void* data[] = { (void*)image, (void*)kernel, NULL };
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint) };
//	size_t length_height_data[] = { height ,  height_kernel , height };
//	size_t length_width_data[] = { width , width_kernel , width };
//	size_t length_row_pitch_data[] = { width * sizeof(cl_uchar4), width_kernel * sizeof(cl_float4), width * sizeof(cl_uchar4) };
//	size_t type_image[] = { CL_RGBA, CL_RGBA, CL_RGBA };
//	size_t type_data[] = { CL_UNSIGNED_INT8, CL_FLOAT, CL_UNSIGNED_INT8 };
//	cl_uint indices_memory[3];
//	cl_uint index_kernel_buffer[] = { 0, 3, 6 };
//	cl_uint index_kernel_arguments[] = { 1, 2, 4, 5 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height , width_kernel, height_kernel };
//	size_t indices = device->mallocImageMemory((const void**)data, length_height_data, length_width_data, length_row_pitch_data, 3, type_image, type_data);
//	indices_memory[0] = indices;
//	indices_memory[1] = indices + 1;
//	indices_memory[2] = indices + 2;
//	device->setArguments(index_kernel, NULL, NULL, indices_memory, 3, index_kernel_buffer, arguments, type_arguments, 4, index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	number_buffers += 3;
//	length_last_buffer = 3;
//	const char* buffers[3] = { "image_image2d" + 0,"kernel_image2d" + 0, "convolution_image2d" + 0 };
//	const size_t length_buffers[] = { 14, 15, 19 };
//	types_buffers_OpenCL = (cl_uchar**)realloc(types_buffers_OpenCL, number_buffers * sizeof(cl_uchar*));
//	length_buffer_OpenCL = (size_t*)realloc(length_buffer_OpenCL, number_buffers * sizeof(size_t));
//	indices_buffer_OpenCL = (size_t*)realloc(indices_buffer_OpenCL, number_buffers * sizeof(size_t));
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t i = number_buffers - 3, j = 0; i < number_buffers; i++, j++) {
//		types_buffers_OpenCL[i] = (cl_uchar*)malloc(length_buffers[j] * sizeof(char));
//
//		memcpy(types_buffers_OpenCL[i], buffers[j], length_buffers[j] * sizeof(char));
//		indices_buffer_OpenCL[i] = indices + j;
//		length_buffer_OpenCL[i] = length_buffers[j];
//		last_buffers_modify[j] = i;
//	}
//}
//void Lab_2::call_opencl_function(size_t index_kernel, clDevice* device, cl_uint* indices_images, cl_uint* indices_arguments, size_t number_images, size_t number_arguments) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint* index_kernel_buffer = (cl_uint*)_alloca(number_images*sizeof(cl_uint));
//	cl_uint* index_kernel_arguments = (cl_uint*)_alloca(number_arguments * sizeof(cl_uint));
//	cl_uint* arguments = (cl_uint*)_alloca(number_arguments * sizeof(cl_uint));
//	size_t i = 0;
//	for (; i < number_images; i++)
//		index_kernel_buffer[i] = i;
//	for (size_t j = 0; i < number_images + number_arguments; i++) {
//		index_kernel_arguments[j++] = i;
//	}
//	i = 0;
//	for (; i < number_arguments; i++)
//		arguments[i] = indices_arguments[i];
//	size_t work_size[] = { indices_arguments[0], indices_arguments[1], 1 };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, number_images, index_kernel_buffer, arguments, type_arguments, number_arguments, index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = number_images;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t j = 0; j < length_last_buffer; j++) {
//		last_buffers_modify[j] = indices_images[j];
//	}
//}
//void Lab_2::convert_float4_to_uchar4_image(size_t index_kernel, clDevice* device, size_t width, size_t height, cl_uint indices_images[2]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1, };
//	cl_uint index_kernel_arguments[] = { 2, 3 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 2, index_kernel_buffer, arguments, type_arguments, 2, index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 2;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t j = 0; j < length_last_buffer; j++) {
//		last_buffers_modify[j] = indices_images[j];
//	}
//}
//void Lab_2::convolution_rgba_device(size_t index_kernel, clDevice* device, size_t width, size_t height, size_t width_kernel, size_t height_kernel, cl_uint indices_images[3]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1, 2 };
//	cl_uint index_kernel_arguments[] = { 3, 4, 5, 6 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height , width_kernel, height_kernel };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 3, index_kernel_buffer, arguments, type_arguments, 4, index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 3;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t j = 0; j < 3; j++) {
//		last_buffers_modify[j] = indices_images[j];
//	}
//}
//
//void Lab_2::make_noise_image_rgba_device(size_t index_kernel, clDevice* device, size_t width, size_t height, cl_uint indices_images[2]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1 };
//	cl_uint index_kernel_arguments[] = { 2, 3 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 2, index_kernel_buffer, arguments, type_arguments, sizeof(index_kernel_arguments) / sizeof(cl_uint), index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 2;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	last_buffers_modify[0] = indices_images[0];
//	last_buffers_modify[1] = indices_images[1];
//}
//
//void Lab_2::divide_fourier_transform_rgba_image_device(size_t index_kernel, clDevice* device, size_t width, size_t height, size_t width_kernel, size_t height_kernel, cl_uint indices_images[6]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint), sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1, 2, 3, 4, 5 };
//	cl_uint index_kernel_arguments[] = { 6, 7, 8, 9 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height, width_kernel, height_kernel };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 6, index_kernel_buffer, arguments, type_arguments, sizeof(index_kernel_arguments) / sizeof(cl_uint), index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 6;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t i = 0; i < length_last_buffer; i++)
//		last_buffers_modify[i] = indices_images[i];
//}
//
//void Lab_2::fourier_transform_rgba_image_device(size_t index_kernel, clDevice* device, size_t width, size_t height, cl_uint indices_images[3]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1, 2 };
//	cl_uint index_kernel_arguments[] = { 3, 4 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 3, index_kernel_buffer, arguments, type_arguments, sizeof(index_kernel_arguments) / sizeof(cl_uint), index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 3;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	last_buffers_modify[0] = indices_images[0];
//	last_buffers_modify[1] = indices_images[1];
//	last_buffers_modify[2] = indices_images[2];
//}
//
//void Lab_2::inverse_fourier_transform_rgba_image_device(size_t index_kernel, clDevice* device, size_t width, size_t height, cl_uint indices_images[3]) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint),  sizeof(cl_uint) };
//	cl_uint index_kernel_buffer[] = { 0, 1, 2 };
//	cl_uint index_kernel_arguments[] = { 3, 4 };
//	size_t work_size[] = { width, height, 1 };
//	cl_uint arguments[] = { width,  height };
//	device->setArguments(index_kernel, NULL, NULL, indices_images, 3, index_kernel_buffer, arguments, type_arguments, sizeof(index_kernel_arguments) / sizeof(cl_uint), index_kernel_arguments);
//	device->startCalculate(index_kernel, work_size);
//	length_last_buffer = 3;
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	last_buffers_modify[0] = indices_images[0];
//	last_buffers_modify[1] = indices_images[1];
//	last_buffers_modify[2] = indices_images[2];
//}
//
//
//
//size_t Lab_2::mallocMemoryImage(clDevice* device, const void* ptr, size_t width, size_t height, size_t type_data, size_t type_image) {
//	size_t _type_image[] = { type_image };
//	size_t _type_data[] = { type_data };
//	size_t length_row_pitch_data[1];
//	size_t length_height_data[] = { height };
//	size_t length_width_data[] = { width };
//	if (type_data == CL_FLOAT && type_image == CL_RGBA)
//		length_row_pitch_data[0] = { width * sizeof(cl_float4) };
//	if (type_data == CL_UNORM_INT8 && type_image == CL_RGBA)
//		length_row_pitch_data[0] = { width * sizeof(cl_uchar4) };
//	else if (type_data == CL_UNSIGNED_INT8 && type_image == CL_RGBA)
//		length_row_pitch_data[0] = { width * sizeof(cl_uchar4) };
//	size_t _indices = device->mallocImageMemory(&ptr, length_height_data, length_width_data, length_row_pitch_data, 1, &_type_image[0], &_type_data[0]);
//	const char* buffers[1] = { "image2d" + 0 };
//	const size_t length_buffers[] = { 9 };
//	number_buffers += 1;
//	length_last_buffer = 1;
//	types_buffers_OpenCL = (cl_uchar**)realloc(types_buffers_OpenCL, number_buffers * sizeof(cl_uchar*));
//	length_buffer_OpenCL = (size_t*)realloc(length_buffer_OpenCL, number_buffers * sizeof(size_t));
//	indices_buffer_OpenCL = (size_t*)realloc(indices_buffer_OpenCL, number_buffers * sizeof(size_t));
//	last_buffers_modify = (cl_uint*)realloc(last_buffers_modify, length_last_buffer * sizeof(cl_uint));
//	for (size_t i = number_buffers - 1, j = 0; i < number_buffers; i++, j++) {
//		types_buffers_OpenCL[i] = (cl_uchar*)malloc(length_buffers[j] * sizeof(char));
//		memcpy(types_buffers_OpenCL[i], buffers[j], length_buffers[j] * sizeof(char));
//		indices_buffer_OpenCL[i] = _indices + j;
//		length_buffer_OpenCL[i] = length_buffers[j];
//		last_buffers_modify[j] = i;
//	}
//	return _indices;
//}
//void Lab_2::readImage(clDevice* device, void* returnData, size_t width_image, size_t height_image, cl_uint index_buffer) {
//	cl_uchar type_arguments[] = { sizeof(cl_uint) };
//	device->readImage((void**)&returnData, &index_buffer, type_arguments, &width_image, &height_image, 1);
//}
InverseFilter::InverseFilter(clDevice* device, cl_uchar4* image, size_t width, size_t height, cl_float4* kernel, size_t width_kernel, size_t height_kernel)
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
	inverse_fourier_transform_local_kernel_index = device->findKernel((const cl_char*)"inverse_fourier_transform_local_memory_rgba_image", sizeof("inverse_fourier_transform_local_memory_rgba_image"));
	cl_int winner_float4_kernel_index = device->findKernel((const cl_char*)	"winner_approximation_rgba", sizeof("winner_approximation_rgba"));
	cl_int div_images_kernel_index = device->findKernel((const cl_char*)		"div_image_rgba_image", sizeof("div_image_rgba_image"));
	cl_int mul_images_kernel_index = device->findKernel((const cl_char*)		"mul_image_rgba_image", sizeof("mul_image_rgba_image"));
	cl_int add_images_kernel_index = device->findKernel((const cl_char*)		"add_image_rgba_image", sizeof("add_image_rgba_image"));
	cl_int mul_const_images_kernel_index = device->findKernel((const cl_char*)		"mul_number_image_rgba_image", sizeof("mul_number_image_rgba_image"));
	cl_int div_const_images_kernel_index = device->findKernel((const cl_char*)		"div_number_image_rgba_image", sizeof("div_number_image_rgba_image"));

	cl_uint image_gpu = device->mallocImageMemory((const void*)image, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint result_image_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Re_image_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Im_image_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Re_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Im_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Re_result_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint norm_Im_result_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);

	cl_uint real_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint real_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint spectrum_image_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint spectrum_kernel_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint spectrum_result_gpu = device->mallocImageMemory(null_ptr, height, height, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint kernel_gpu = device->mallocImageMemory((const void*)kernel, height_kernel, width_kernel, length_row_pitch_data[2], CL_RGBA, CL_FLOAT);
	cl_uint copy_kernel_gpu = device->mallocImageMemory(null_ptr, height_kernel, width_kernel, length_row_pitch_data[ 2], CL_RGBA, CL_FLOAT);

	cl_uint real_result_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);
	cl_uint imagine_result_gpu = device->mallocImageMemory(null_ptr, height, width, *length_row_pitch_data, CL_RGBA, CL_FLOAT);

	cl_uint mean_kernel_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);
	cl_uint max_im_image_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);
	cl_uint max_re_image_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);
	cl_uint max_im_kernel_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);
	cl_uint max_re_kernel_gpu = device->mallocImageMemory(null_ptr, one_to_one_image, one_to_one_image, length_row_pitch_data[4], CL_RGBA, CL_FLOAT);

	{
		cl_uint indices[] = { image_gpu, real_image_gpu, imagine_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(fourier_transform_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	/*getMeanOrMax(real_image_gpu, max_re_image_gpu, width, height, false);
	getMeanOrMax(imagine_image_gpu, max_im_image_gpu, width, height, false);
*/
	{
		cl_uint indices[] = { kernel_gpu, real_kernel_gpu, imagine_kernel_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(fourier_transform_float4_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	//getMeanOrMax(real_kernel_gpu, max_re_kernel_gpu, width, height, false);
	//getMeanOrMax(imagine_kernel_gpu, max_im_kernel_gpu, width, height, false);
	/*{
		cl_uint indices[] = { real_image_gpu, max_re_image_gpu, real_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}

	{
		cl_uint indices[] = { imagine_image_gpu, max_im_image_gpu, imagine_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}*/
	//{
	//	cl_uint indices[] = { real_kernel_gpu, max_re_kernel_gpu, real_kernel_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}

	//{
	//	cl_uint indices[] = { imagine_kernel_gpu, max_im_kernel_gpu, imagine_kernel_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}

	{
		cl_uint indices[] = { real_image_gpu, real_kernel_gpu, real_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(div_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);

	}
	{
		cl_uint indices[] = { imagine_image_gpu, imagine_kernel_gpu, imagine_image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(div_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	//getMeanOrMax(real_image_gpu, max_re_image_gpu, width, height, false);
	//getMeanOrMax(imagine_image_gpu, max_im_image_gpu, width, height, false);
	//{
	//	cl_uint indices[] = { imagine_image_gpu,max_im_image_gpu , imagine_image_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}
	//{
	//	cl_uint indices[] = { real_image_gpu, max_re_image_gpu, real_image_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(div_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}
	//{
	//	cl_uint indices[] = { norm_Re_image_gpu, norm_Re_result_gpu, real_image_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(mul_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}
	//{
	//	cl_uint indices[] = { norm_Re_result_gpu, norm_Im_image_gpu, imagine_image_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(mul_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	//}
	{
		cl_uint indices[] = { real_image_gpu, imagine_image_gpu, result_image_gpu };
		cl_uint indices_args[] = { width, height, NULL };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint), -(int)_device->DeviceInfo.maxWorkGroupSize * 16 };
		device->callOpenclFunction(inverse_fourier_transform_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);
	}
	//getMean(image_gpu, mean_kernel_gpu, width, height);
	//{
	//	cl_uint indices[] = { mean_image_gpu, mean_kernel_gpu, mean_image_gpu };
	//	cl_uint indices_args[] = { 1, 1 };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(div_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);

	//}
	//{
	//	cl_uint indices[] = { result_image_gpu, mean_kernel_gpu, result_image_gpu };
	//	cl_uint indices_args[] = { width, height };
	//	cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
	//	device->callOpenclFunction(mul_const_images_kernel_index, indices, (cl_char*)indices_args, length_args, 3, 2);

	//}
	
	
	device->readImage((void**)&image, &result_image_gpu, type_arguments, &width, &height, 1);
	for (size_t i = max_re_kernel_gpu; i >= image_gpu; i--)
		device->freeImageMemory(i);

}

InverseFilter::~InverseFilter()
{
}