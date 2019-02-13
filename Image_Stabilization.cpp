#include "Image_Stabilization.h"

Image_Stabilization::Image_Stabilization(clDevice* device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y)
{
	_device = device;
	length_data = width * height * sizeof(cl_uchar4);
	size_t length_row_pitch_data = width * sizeof(cl_uchar4);
	kernel_image_gauss = device->findKernel((const cl_char*)"make_gauss_vec1_image_uchar_rgba", sizeof("make_gauss_vec1_image_uchar_rgba"));
	kernel_image_stabilization = device->findKernel((const cl_char*)"image_stabilization_vec1_image_rgba", sizeof("image_stabilization_vec1_image_rgba"));
	norm_image_gpu_0 = device->mallocImageMemory(NULL, height, width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	norm_image_gpu_1 = device->mallocImageMemory(NULL, height, width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	memory_buffer = device->mallocBufferMemory(NULL, length_data);
	args_gauss_indices[0] = width, args_gauss_indices[1] = height, args_gauss_indices[2] = block_x, args_gauss_indices[3] = block_y;
	args_conv_indices[0] = width, args_conv_indices[1] = height, args_conv_indices[4] = block_x, args_conv_indices[5] = block_y, args_conv_indices[6] = NULL;
	length_gauss_args[0] = sizeof(cl_uint), length_gauss_args[1] = sizeof(cl_uint), length_gauss_args[2] = sizeof(cl_uint), length_gauss_args[3] = sizeof(cl_uint);

	length_conv_args[0] = sizeof(cl_uint), length_conv_args[1] = sizeof(cl_uint), length_conv_args[2] = sizeof(cl_uint), length_conv_args[3] = sizeof(cl_uint),
	length_conv_args[4] = sizeof(cl_uint) , length_conv_args[5] = sizeof(cl_uint), length_conv_args[6] = -(int)(_device->DeviceInfo.maxWorkGroupSize + _device->DeviceInfo.maxWorkGroupSize/(block_x*block_y)) * sizeof(cl_float) ;
}
void Image_Stabilization::Calculate_Gauss_function(void* data, void* result) {
	size_t work_size[3] = { args_gauss_indices[0], args_gauss_indices[1], 1 };
	_device->write2DImage(data, norm_image_gpu_0, args_gauss_indices[0], args_gauss_indices[1]);
//	_device->callOpenclFunction(kernel_image_gauss, &memory_buffer, &norm_image_gpu_0, (cl_char*)args_gauss_indices, length_gauss_args, 1, 1, 4, work_size);
//	_device->readBuffer(result, memory_buffer, length_data);
}
void Image_Stabilization::Stabilization_function(void* data_next_image, void* result, size_t width_current, size_t height_current) {
	size_t globalWork[3] = { width_current, height_current, 1 }; 
	args_conv_indices[2] = width_current;
	args_conv_indices[3] = height_current;
	size_t x = sqrt(_device->kernelInfo[kernel_image_stabilization].max_work_group_size);
	size_t y = _device->kernelInfo[kernel_image_stabilization].max_work_group_size / x;
	x = x - (x %  args_conv_indices[4]);
	y = y - (y %  args_conv_indices[5]);
	size_t localWork[3] = { x, y, 1 };
	if (globalWork[0] % localWork[0])
		globalWork[0] += localWork[0] - globalWork[0] % localWork[0];
	if (globalWork[1] % localWork[1])
		globalWork[1] += localWork[1] - globalWork[1] % localWork[1];
	if (globalWork[2] % localWork[2])
		globalWork[2] += localWork[2] - globalWork[2] % localWork[2];
	cl_uint images[] = { norm_image_gpu_0, norm_image_gpu_1 };
	_device->write2DImage(data_next_image, norm_image_gpu_1, args_conv_indices[0], args_conv_indices[1]);

	_device->callOpenclFunction(kernel_image_stabilization, &memory_buffer, images, (cl_char*)args_conv_indices, length_conv_args, 1, 2, 7, globalWork, localWork);
	_device->readBuffer(result, memory_buffer, length_data);
}
Image_Stabilization::~Image_Stabilization()
{
	_device->freeImageMemory(norm_image_gpu_0);
	_device->freeImageMemory(norm_image_gpu_1);
	_device->popBufferMemory();

}