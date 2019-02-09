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

	cl_uint result = device->mallocBufferMemory(NULL, frames[0].height* frames[0].width * sizeof(cl_float));
	{
		cl_uint block_x = 2;
		cl_uint block_y = 2;
		cl_uint indices[] = { norm_image_gpu[0], norm_image_gpu[1], result };
		cl_uint indices_args[] = { frames[0].width, frames[0].height, frames[1].width, frames[1].height, block_x, block_y };
		cl_int length_args[] = { sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(kernel_image_stabilization, indices, (cl_char*)indices_args, length_args, sizeof(indices) / sizeof(cl_uint), sizeof(length_args) / sizeof(cl_int));
	}
	device->popBufferMemory();
	for (ptrdiff_t i = frames.size() - 1; i != -1; i--) {
		device->freeImageMemory(norm_image_gpu[i]);
	}
}

Image_Stabilization::~Image_Stabilization()
{
}