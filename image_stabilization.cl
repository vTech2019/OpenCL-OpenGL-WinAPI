R"===(
#define PI 3.14159265359f

float Gaussian_filter_x(float x, float sigma, float mean){
	const float _x = x - mean;
	const float _sigma = 2*sigma;
	const float div = _x / _sigma;
	return native_exp(-div * div)/(sigma * sqrt(2.0f * PI))
}

float Gaussian_filter_y(float y, float sigma, float mean){
	const float _y = y - mean;
	const float _sigma = 2*sigma;
	const float div = _y / _sigma;
	return native_exp(-div * div)/(sigma * sqrt(2.0f * PI))
}
float Gaussian_filter_xy(float y, float x, float sigma){
	const float _y = y * y;
	const float _x = x * x;
	const float _sigma = 2.0f*sigma*sigma;
	const float div = (_y + _x) / _sigma;
	return native_exp(-div)/(_sigma * PI)
}
__kernel void image_stabilization_float4_image_rgba(read_only image2d_t image_current, read_only image2d_t image_next, float* result_buffer, const int width_current,const int height_current,  int width_next, int height_next, int block_x, int block_y){

    int idx = get_global_id(0);
    int idy = get_global_id(1);
	float f_block_x = convert_float(block_x);
	float f_block_y = convert_float(block_y);
	for (int h = 0; h < height_current; h++){
		for (int w = 0; w <  width_current; w++){
			for (float i = 0.0f; i < f_block_y; i+=1.0f){
				for (float j = 0.0f; j < f_block_x; j+=1.0f){
					result_buffer[h * width_current + w] = Gaussian_filter_xy(i, j, 
				}
			}
		}
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