#include "Image_Stabilization.h"

Image_Stabilization::Image_Stabilization(cpuDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y, cl_uint radius)
{
	_gpu_data = NULL;
	_cpu_data = (cpu_data*)_malloca(sizeof(cpu_data));
	_cpu_data->ptrDevice = device;
	_cpu_data->block_x = block_x;
	_cpu_data->block_y = block_y;
	_cpu_data->height_image = height;
	_cpu_data->width_image = width;
	if (device->support_avx512F)
		_cpu_data->current_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m512)),
		_cpu_data->next_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m512));
	else if (device->support_avx)
		_cpu_data->current_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m256)),
		_cpu_data->next_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m256));
	else if (device->support_sse1)
		_cpu_data->current_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m128)),
		_cpu_data->next_image = (cl_float4*)_mm_malloc(width * height * sizeof(cl_float4), sizeof(__m128));
	else
		_cpu_data->current_image = (cl_float4*)malloc(width * height * sizeof(cl_float4)),
		_cpu_data->next_image = (cl_float4*)malloc(width * height * sizeof(cl_float4));
	ptr_gauss_function = &Image_Stabilization::cpu_Calculate_Gauss_function;
	ptr_stabilization_function = &Image_Stabilization::cpu_Calculate_stabilization_function;
}

void Image_Stabilization::cpu_Calculate_Gauss_function(void* data, void* result, size_t width_current, size_t height_current) {
	cl_float4* ptr_memory_image = _cpu_data->current_image;
	cl_uchar4* ptr_image = (cl_uchar4*)data;
	size_t width = _cpu_data->width_image;
	size_t height = _cpu_data->height_image;
	size_t last_data = width % 4;
	size_t width_image = width;
	width /= 4;
	static const __m128 magicFloat = _mm_set1_ps(8388608.0f);
	static const __m128i magicInt = _mm_set1_epi16(0x4B00);
	//for (size_t h = 0; h < height; h++) {
	//	__m128i* ptr_vector_image = (__m128i*)(ptr_image + h * width_image);
	//	float* ptr_vector_result = (float*)(ptr_memory_image + h * width_image);
	//	for (size_t w = 0; w < width; w++) {
	//		__m128i xmm0 = _mm_loadu_si128(ptr_vector_image++);
	//		__int64 timer = __rdtsc();
	//		__m128i xmm1 = _mm_cvtepu8_epi16(xmm0);
	//		__m128i xmm2 = _mm_bsrli_si128(xmm0, 8);
	//		__m128i xmm3 = _mm_cvtepu8_epi16(xmm2);
	//		
	//		__m128i xmm4 = _mm_unpacklo_epi16(xmm1, magicInt);
	//		__m128i xmm5 = _mm_unpackhi_epi16(xmm1, magicInt);
	//		__m128 xmm6 = _mm_sub_ps(_mm_castsi128_ps(xmm4), magicFloat);
	//		__m128 xmm7 = _mm_sub_ps(_mm_castsi128_ps(xmm5), magicFloat);
	//		timer = __rdtsc() - timer;
	//		printf("%zi\n", timer);
	//		_mm_store_ps(ptr_vector_result, xmm6);
	//		_mm_store_ps(ptr_vector_result + 4, xmm7);
	//	} 
	//}
}
void Image_Stabilization::cpu_Calculate_stabilization_function(void* data, void* result, size_t width_current, size_t height_current) {

}
Image_Stabilization::Image_Stabilization(clDevice* device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y)
{
	_cpu_data = NULL;
	_gpu_data = (gpu_data*)_malloca(sizeof(gpu_data));
	_gpu_data->_device = device;
	_gpu_data->kernel_image_gauss = device->findKernel((const cl_char*)"make_gauss_vec1_image_uchar_rgba", sizeof("make_gauss_vec1_image_uchar_rgba"));
	_gpu_data->kernel_image_stabilization = device->findKernel((const cl_char*)"image_stabilization_vec1_image_rgba", sizeof("image_stabilization_vec1_image_rgba"));
	size_t x = sqrt(_gpu_data->_device->kernelInfo[_gpu_data->kernel_image_stabilization].max_work_group_size);
	size_t y = _gpu_data->_device->kernelInfo[_gpu_data->kernel_image_stabilization].max_work_group_size / x;
	x = x - (x %  block_x);
	y = y - (y %  block_y);
	_gpu_data->localWork[0] = x, _gpu_data->localWork[1] = y, _gpu_data->localWork[2] =  1;
	_gpu_data->globalWork[0] = width, _gpu_data->globalWork[1] = height, _gpu_data->globalWork[2] = 1 ;
	if (_gpu_data->globalWork[0] % _gpu_data->localWork[0])
		_gpu_data->globalWork[0] += _gpu_data->localWork[0] - _gpu_data->globalWork[0] % _gpu_data->localWork[0];
	if (_gpu_data->globalWork[1] % _gpu_data->localWork[1])
		_gpu_data->globalWork[1] += _gpu_data->localWork[1] - _gpu_data->globalWork[1] % _gpu_data->localWork[1];
	if (_gpu_data->globalWork[2] % _gpu_data->localWork[2])
		_gpu_data->globalWork[2] += _gpu_data->localWork[2] - _gpu_data->globalWork[2] % _gpu_data->localWork[2];
	int origin_width = width;
	int origin_height = height;
	width = _gpu_data->globalWork[0];
	height = _gpu_data->globalWork[1];
	_gpu_data->length_data = width * height * sizeof(cl_uchar4);
	size_t length_row_pitch_data = width * sizeof(cl_uchar4);
	_gpu_data->norm_image_gpu_0 = device->mallocImage2DMemory(NULL, height, width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	_gpu_data->norm_image_gpu_1 = device->mallocImage2DMemory(NULL, height, width, length_row_pitch_data, CL_RGBA, CL_UNORM_INT8);
	_gpu_data->memory_buffer = device->mallocBufferMemory(NULL, _gpu_data->length_data);
	_gpu_data->args_gauss_indices[0] = origin_width, _gpu_data->args_gauss_indices[1] = origin_height, _gpu_data->args_gauss_indices[2] = block_x, _gpu_data->args_gauss_indices[3] = block_y;
	_gpu_data->args_conv_indices[0] = origin_width, _gpu_data->args_conv_indices[1] = origin_height, _gpu_data->args_conv_indices[4] = block_x, _gpu_data->args_conv_indices[5] = block_y, _gpu_data->args_conv_indices[6] = NULL;
	_gpu_data->length_gauss_args[0] = sizeof(cl_uint), _gpu_data->length_gauss_args[1] = sizeof(cl_uint), _gpu_data->length_gauss_args[2] = sizeof(cl_uint), _gpu_data->length_gauss_args[3] = sizeof(cl_uint);

	int size_local_memory = (x*y);
	_gpu_data->length_conv_args[0] = sizeof(cl_uint), _gpu_data->length_conv_args[1] = sizeof(cl_uint), _gpu_data->length_conv_args[2] = sizeof(cl_uint), _gpu_data->length_conv_args[3] = sizeof(cl_uint),
	_gpu_data->length_conv_args[4] = sizeof(cl_uint) , _gpu_data->length_conv_args[5] = sizeof(cl_uint), _gpu_data->length_conv_args[6] = -(int)(size_local_memory) * sizeof(cl_float) ;
	ptr_gauss_function = &Image_Stabilization::gpu_Calculate_Gauss_function;
}
void Image_Stabilization::gpu_Calculate_Gauss_function(void* data, void* result, size_t width_current, size_t height_current) {
	size_t work_size[3] = { _gpu_data->args_gauss_indices[0], _gpu_data->args_gauss_indices[1], 1 };

	size_t length_row_pitch_data = width_current * sizeof(cl_uchar4);
	_gpu_data->_device->write2DImage(data, _gpu_data->norm_image_gpu_0, width_current, height_current);
//	_device->callOpenclFunction(kernel_image_gauss, &memory_buffer, &norm_image_gpu_0, (cl_char*)args_gauss_indices, length_gauss_args, 1, 1, 4, work_size);
//	_device->readBuffer(result, memory_buffer, length_data);
}
void Image_Stabilization::gpu_Stabilization_function(void* data_next_image, void* result, size_t width_current, size_t height_current) {
	_gpu_data->args_conv_indices[2] = width_current;
	_gpu_data->args_conv_indices[3] = height_current;
	size_t images[] = { _gpu_data->norm_image_gpu_0, _gpu_data->norm_image_gpu_1 };
	_gpu_data->_device->write2DImage(data_next_image, _gpu_data->norm_image_gpu_1, width_current, height_current);
	
	_gpu_data->_device->callOpenclFunction(_gpu_data->kernel_image_stabilization, &_gpu_data->memory_buffer, images, (cl_char*)_gpu_data->args_conv_indices, _gpu_data->length_conv_args, 1, 2, 7, _gpu_data->globalWork, _gpu_data->localWork);
	_gpu_data->_device->readBuffer(result, _gpu_data->memory_buffer, width_current*height_current*sizeof(cl_uchar4));
}
void Image_Stabilization::Calculate_Gauss_function(void* data, void* result, size_t width_current, size_t height_current) {
	(this->*ptr_gauss_function)(data, result, width_current, height_current);
}
void Image_Stabilization::Stabilization_function(void* data_next_image, void* result, size_t width_current, size_t height_current) {
	(this->*ptr_stabilization_function)(data_next_image, result, width_current, height_current);
}
Image_Stabilization::~Image_Stabilization()
{
	if (_gpu_data) {
		_gpu_data->_device->freeMemory(_gpu_data->norm_image_gpu_0);
		_gpu_data->_device->freeMemory(_gpu_data->norm_image_gpu_1);
		_gpu_data->_device->freeMemory(_gpu_data->memory_buffer);
		_freea(_gpu_data);
	}
	if (_cpu_data) {
		if (_cpu_data->current_image)
			if (_cpu_data->ptrDevice->support_sse1)
				_mm_free(_cpu_data->current_image);
			else
				free(_cpu_data->current_image);
		if (_cpu_data->next_image)
			if (_cpu_data->ptrDevice->support_sse1)
				_mm_free(_cpu_data->next_image);
			else
				free(_cpu_data->next_image);
		_freea(_cpu_data);
	}
}