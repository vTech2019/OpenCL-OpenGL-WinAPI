#include "Image_Stabilization.h"

Image_Stabilization::Image_Stabilization(clDevice* device, std::vector<dataImage> frames, std::vector<dataImage> result_frames)
{
	cl_uchar type_arguments[] = { sizeof(cl_uint) };
	cl_uint* norm_image_gpu = (cl_uint*)alloca(frames.size() * sizeof(cl_uint));
	for (ptrdiff_t i = 0; i < frames.size(); i++) {
		size_t length_row_pitch_data = frames[i].width * sizeof(cl_uchar4);
		norm_image_gpu[i] = device->mallocImageMemory((const void*)frames[i].data, frames[i].height, frames[i].width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	}
	kernel_image_stabilization = device->findKernel((const cl_char*)"image_stabilization_float4_image_rgba", sizeof("Image_Stabilization_float4_image_rgba"));
	cl_uint result = device->mallocBufferMemory(NULL, frames[0].height* frames[0].width * sizeof(cl_uchar4));
	{
		cl_uint block_x = 7;
		cl_uint block_y = 7;
		cl_uint image_indices[] = { norm_image_gpu[0], norm_image_gpu[1] };
		cl_uint buffer_indices[] = { result };
		cl_uint args_indices[] = { frames[0].width, frames[0].height, frames[1].width, frames[1].height, block_x, block_y };
		size_t work_size[] = { frames[0].width/ block_x, frames[0].height / block_y, 1};
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(kernel_image_stabilization, buffer_indices, image_indices, (cl_char*)args_indices, length_args, sizeof(buffer_indices) / sizeof(cl_uint), sizeof(image_indices) / sizeof(cl_uint), sizeof(length_args) / sizeof(cl_int), work_size);
	}
	device->popBufferMemory();
	for (ptrdiff_t i = frames.size() - 1; i != -1; i--) {
		device->freeImageMemory(norm_image_gpu[i]);
	}
}

Image_Stabilization::~Image_Stabilization()
{
}