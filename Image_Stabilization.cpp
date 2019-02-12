#include "Image_Stabilization.h"

Image_Stabilization::Image_Stabilization(clDevice* device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y)
{
	_device = device;
	length_data = width * height * sizeof(cl_uchar4);
	size_t length_row_pitch_data = width * sizeof(cl_uchar4);
	kernel_image_gauss = device->findKernel((const cl_char*)"make_gauss_vec1_image_uchar_rgba", sizeof("make_gauss_vec1_image_uchar_rgba"));
	kernel_image_stabilization = device->findKernel((const cl_char*)"image_stabilization_vec1_image_rgba", sizeof("image_stabilization_vec1_image_rgba"));
	norm_image_gpu = device->mallocImageMemory(NULL, height, width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	memory_buffer = device->mallocBufferMemory(NULL, length_data);
	args_indices[0] =  width , args_indices[1] =  height, args_indices[2] = block_x, args_indices[3] = block_y;
	work_size[0] = width / block_x, work_size[1] = height / block_y, work_size[2] = 1 ;
	length_args[0] = sizeof(cl_uint), length_args[1] = sizeof(cl_uint), length_args[2] = sizeof(cl_uint), length_args[3] = sizeof(cl_uint);
}
void Image_Stabilization::Calculate_Gauss_function(void* data, void* result) {
	_device->write2DImage(data, norm_image_gpu, args_indices[0], args_indices[1]);

	//_device->writeBuffer(data, memory_buffer, length_data);
	_device->callOpenclFunction(kernel_image_gauss, &memory_buffer, &norm_image_gpu, (cl_char*)args_indices, length_args, 1, 1, 4, work_size);
	//_device->readBuffer(result, memory_buffer, length_data);
//	_device->copy2DImageToBuffer(norm_image_gpu, memory_buffer, args_indices[0], args_indices[1]);
	//_device->readImage(data, norm_image_gpu, args_indices[0], args_indices[1]);
	_device->readBuffer(data, memory_buffer, length_data);
}
Image_Stabilization::~Image_Stabilization()
{
	_device->freeImageMemory(norm_image_gpu);
	_device->popBufferMemory();

}