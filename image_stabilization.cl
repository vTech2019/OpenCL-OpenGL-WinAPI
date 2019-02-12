R"===(
#define PI 3.14159265359f

float Gaussian_filter_x(float x, float sigma, float mean){
	const float _x = x - mean;
	const float _sigma = 2*sigma;
	const float div = _x / _sigma;
	return native_exp(-div * div)/(sigma * sqrt(2.0f * PI));
}

float Gaussian_filter_y(float y, float sigma, float mean){
	const float _y = y - mean;
	const float _sigma = 2*sigma;
	const float div = _y / _sigma;
	return native_exp(-div * div)/(sigma * sqrt(2.0f * PI));
}
float Gaussian_filter_xy(float y, float x, float sigma){
	const float _y = y * y;
	const float _x = x * x;
	const float _sigma = 2.0f*sigma*sigma;
	const float div = (_y + _x) / _sigma;
	return native_exp(-div)/(_sigma * PI);
}
__kernel void make_gauss_vec1_image_uchar_rgba(__global uchar4* result_buffer, read_only image2d_t image, const int width,const int height, int block_x, int block_y){
    int idx = get_global_id(0);
    int idy = get_global_id(1);
	int steps_width_current = width / block_x;
	int steps_height_current = height / block_y;
	float w = 4.0f * sqrt(2.0f * log(2.0f));
	float sigma = block_x / w;
	float norm = block_x * block_y; 
	int part_block_x = (block_x - 1)/ 2; 
	int part_block_y = (block_y - 1) / 2; 
    int stride_x = get_global_size(0);
    int stride_y = get_global_size(1);
	for (int h = idy; h < steps_height_current; h+=stride_y)
		for (int w = idx; w <  steps_width_current; w+=stride_x){
			h *= block_y;
			w *= block_x;
			for (int i = -part_block_y; i <= part_block_y; i++){
				for (int j = -part_block_x; j <= part_block_x; j++){
					const float4 current_image_block = read_imagef(image, (int2)(w + j + part_block_x, h + i + part_block_y)) * (float4) norm * Gaussian_filter_xy(convert_float(i), convert_float(j), sigma);
					result_buffer[(h + i + part_block_y)* width  + (w + j + part_block_x)] = convert_uchar(255.0f * current_image_block);
				}
			}
		}
}
__kernel void image_stabilization_vec1_image_rgba(__global uchar4* result_buffer, read_only image2d_t image_current, read_only image2d_t image_next, const int width_current,const int height_current,  int width_next, int height_next, int block_x, int block_y){
    int idx = get_global_id(0);
    int idy = get_global_id(1);
	int steps_width_current = width_current / block_x;
	int steps_height_current = height_current / block_y;

	int part_block_x = (block_x - 1)/ 2; 
	int part_block_y = (block_y - 1) / 2; 
    int stride_x = get_global_size(0);
    int stride_y = get_global_size(1);
	for (int h = idy; h < steps_height_current; h+=stride_y)
		for (int w = idx; w <  steps_width_current; w+=stride_x){
		h *= block_y;
		w *= block_x;
		float best_max = 0.0f;
		int index_x = w;
		int index_y = h;
			for (int h_n = 0; h < 5; h_n+=1)
				for (int w_n = 0; w <  5; w_n+=1){
					float sum = 0.0f;
					for (int i = -part_block_y; i <= part_block_y; i++)
						for (int j = -part_block_x; j <= part_block_x; j++){
							const float4 current_image_block = read_imagef(image_current, (int2)(w + j + part_block_x, h + i + part_block_y));
							const float4 next_image_block = read_imagef(image_next, (int2)(w_n + j + part_block_x, h_n + i + part_block_y));
							sum += current_image_block.x * next_image_block.x;
						}
					if (sum > best_max){
						best_max = sum;
						index_x = w_n;
						index_y = h_n;
					}
				}
				for (int i = 0; i < block_y; i++)
					for (int j = 0; j < block_x; j++)
						result_buffer[(h + i) * width_current + (w + j)] = convert_uchar(255.0f * read_imagef(image_next, (int2)(index_x + j, index_y + i)));
		}

}

#ifdef cl_khr_mipmap_image
#pragma OPENCL EXTENSION cl_khr_mipmap_image : enable
__kernel void image_stabilization_float4_image_rgba(read_only image2d_t image_current, read_only image2d_t image_next, write_only image2d_t image_write, const int width_current,const int height_current, const int width_next,const int height_next){

    int idx = get_global_id(0);
    int idy = get_global_id(1);
    int image_current_num_mip_levels = get_image_num_mip_levels(image_current);
    int image_next_num_mip_levels = get_image_num_mip_levels(image_next);


}
#endif

)==="