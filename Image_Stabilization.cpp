#include "Image_Stabilization.h"

Image_Stabilization::Image_Stabilization(cpuDevice * device, cl_uint width, cl_uint height, cl_uint block_x, cl_uint block_y, cl_uint radius)
{
	_gpu_data = NULL;
	_cpu_data = (cpu_data*)_malloca(sizeof(cpu_data));
	_cpu_data->ptrDevice = device;
	_cpu_data->block_x = block_x;
	_cpu_data->block_y = block_y;
	_cpu_data->height_image = height;
	if (device->support_avx512F) {
		_cpu_data->pitch_width_image = width + sizeof(__m512) - (width % sizeof(__m512));
		_cpu_data->width_image = width;
		_cpu_data->current_image = (cl_float4*)_mm_malloc(2 * width * height * sizeof(cl_float4), sizeof(__m512));
		_cpu_data->next_image = _cpu_data->current_image + width * height * sizeof(cl_float4);
	}
	else if (device->support_avx) {
		_cpu_data->pitch_width_image =  width + sizeof(__m256) - (width % sizeof(__m256));
		_cpu_data->width_image = width;
		_cpu_data->current_image = (cl_float4*)_mm_malloc(2 * width * height * sizeof(cl_float4), sizeof(__m256));
		_cpu_data->next_image = _cpu_data->current_image + width * height * sizeof(cl_float4);
	}
	else if (device->support_sse1) {
		_cpu_data->pitch_width_image = width + sizeof(__m128) - (width % sizeof(__m128));
		_cpu_data->width_image = width;
		_cpu_data->current_image = (cl_float4*)_mm_malloc(2 * width * height * sizeof(cl_float4), sizeof(__m128));
		_cpu_data->next_image = _cpu_data->current_image + width * height * sizeof(cl_float4);
	}
	else {
		_cpu_data->current_image = (cl_float4*)malloc(2 * width * height * sizeof(cl_float4));
		_cpu_data->next_image = _cpu_data->current_image + width * height * sizeof(cl_float4);
	}
	ptr_gauss_function = &Image_Stabilization::cpu_Calculate_Gauss_function;
	ptr_stabilization_function = &Image_Stabilization::cpu_Calculate_stabilization_function;
}
__m128 _ExpSse(__m128 x)
{
	const __m128 a = _mm_set1_ps((1 << 22) / 0.693147180559945309417f);  
	const __m128i b = _mm_set1_epi32(127 * (1 << 23));      
	__m128i r = _mm_cvtps_epi32(_mm_mul_ps(a, x));
	__m128i s = _mm_add_epi32(b, r);
	__m128i t = _mm_sub_epi32(b, r);
	return _mm_div_ps(_mm_castsi128_ps(s), _mm_castsi128_ps(t));
}
void Image_Stabilization::cpu_Calculate_Gauss_function(void* data, void* result, size_t width_current, size_t height_current) {
	cl_float4* ptr_memory_image = _cpu_data->current_image;
	cl_uchar4* ptr_image = (cl_uchar4*)data;
	size_t pitch_width = _cpu_data->pitch_width_image;
	size_t width = _cpu_data->width_image;
	size_t height = _cpu_data->height_image;
	size_t last_data = width % 4;
	size_t width_image = width_current;
	size_t block_x = _cpu_data->block_x;
	size_t block_y = _cpu_data->block_y;
	float w = 4.0f * sqrt(2.0f * log(2.0f));
	float sigma = block_x / w;
	float normalize = block_x * block_y;
	float part_block_x = -float((block_x - 1) / 2);
	float part_block_y = -float((block_y - 1) / 2);
	static const __m128 sigma_xmm = _mm_set1_ps(2.0f*sigma*sigma);
	static const __m128 sigma_pi_xmm = _mm_set1_ps(2.0f*sigma*sigma*3.1415926535f);
	static const __m128 normalize_xmm = _mm_set1_ps(normalize);
	static const __m128 part_block_x_xmm = _mm_set1_ps(part_block_x);
	static const __m128 part_block_y_xmm = _mm_set1_ps(part_block_y);
	static const __m128 magicFloat = _mm_set1_ps(8388608.0f);
	static const __m128i magicInt = _mm_set1_epi16(0x4B00);
	double number = 0;
	double _number = 0;
		for (size_t h = 0; h < height; h++) {
			float* ptr_vector_result = (float*)(ptr_memory_image + h * pitch_width);
			__m128i* ptr_vector_image = (__m128i*)(ptr_image + h * width_image);
			const __m128 intex_y = _mm_set1_ps(float(h % block_y));
			for (size_t w = 0; w < width;) {
		__int64 timer = __rdtsc();
				_mm_prefetch((char *)(ptr_vector_image + 1), _MM_HINT_T0);
				_mm_prefetch((char *)(ptr_vector_image + 2), _MM_HINT_T1);
				_mm_prefetch((char *)(ptr_vector_image + 3), _MM_HINT_T2);
				
				__m128 _y = _mm_add_ps(part_block_y_xmm, intex_y);
				__m128 _x = _mm_sub_ps(part_block_x_xmm, _mm_set_ps(w++ % block_x, w++ % block_x, w++ % block_x, w++ % block_x));
				__m128 _y_y = _mm_mul_ps(_y, _y);
				__m128 _x_x = _mm_mul_ps(_x, _x);
				__m128 _div = _mm_div_ps(_mm_add_ps(_x, _y), sigma_xmm);
				__m128 _gauss = _mm_mul_ps(_mm_div_ps(_ExpSse(_div), sigma_pi_xmm), normalize_xmm);
				__m128i xmm0 = _mm_loadu_si128(ptr_vector_image++);
				__m128i xmm1 = _mm_unpacklo_epi8(xmm0, _mm_setzero_si128());
				__m128i xmm2 = _mm_unpackhi_epi8(xmm0, _mm_setzero_si128());
				__m128i xmm3 = _mm_unpacklo_epi16(xmm1, magicInt);
				__m128i xmm4 = _mm_unpackhi_epi16(xmm1, magicInt);
				__m128i xmm5 = _mm_unpacklo_epi16(xmm2, magicInt);
				__m128i xmm6 = _mm_unpackhi_epi16(xmm2, magicInt);
				__m128 f_xmm3 = _mm_sub_ps(_mm_castsi128_ps(xmm3), magicFloat);
				__m128 f_xmm4 = _mm_sub_ps(_mm_castsi128_ps(xmm4), magicFloat);
				__m128 f_xmm5 = _mm_sub_ps(_mm_castsi128_ps(xmm5), magicFloat);
				__m128 f_xmm6 = _mm_sub_ps(_mm_castsi128_ps(xmm6), magicFloat);
				__m128 f_xmm7 = _mm_unpacklo_ps(_gauss, _gauss);
				__m128 f_xmm8 = _mm_unpacklo_ps(_gauss, _gauss);
				__m128 f_xmm9 = _mm_unpackhi_ps(_gauss, _gauss);
				__m128 f_xmm10 = _mm_unpackhi_ps(_gauss, _gauss);
				f_xmm7 = _mm_unpacklo_ps(f_xmm7, f_xmm7);
				_mm_store_ps(ptr_vector_result, _mm_mul_ps(f_xmm3, f_xmm7));
				f_xmm8 = _mm_unpackhi_ps(f_xmm8, f_xmm8);
				_mm_store_ps(ptr_vector_result + 4, _mm_mul_ps(f_xmm4, f_xmm8));
				f_xmm9 = _mm_unpacklo_ps(f_xmm9, f_xmm9);
				_mm_store_ps(ptr_vector_result + 8, _mm_mul_ps(f_xmm5, f_xmm9));
				f_xmm10 = _mm_unpackhi_ps(f_xmm10, f_xmm10);
				_mm_store_ps(ptr_vector_result + 12, _mm_mul_ps(f_xmm6, f_xmm10));
				_number += __rdtsc() - timer;
				number++;
				printf("%f\n", f_xmm7.m128_f32[0]);
				printf("%f\n", f_xmm8.m128_f32[0]);
				printf("%f\n", f_xmm9.m128_f32[0]);
				printf("%f\n", f_xmm10.m128_f32[0]);
				//printf("%f\n", _number/ number);
				ptr_vector_result += 16;
			} 
		}
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
		_freea(_cpu_data);
	}
}