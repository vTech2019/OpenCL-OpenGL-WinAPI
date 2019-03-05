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
	float w = 4.0f * sqrt(2.0f * log(2.0f));
	float sigma = block_x / w;
	float4 norm = block_x * block_y; 
	int part_block_x = (block_x - 1)/ 2; 
	int part_block_y = (block_y - 1) / 2; 
    int stride_x = get_global_size(0);
    int stride_y = get_global_size(1);
	for (int h = idy; h < height; h+=stride_y)
		for (int w = idx; w <  width; w+=stride_x){
			int i =  -part_block_y + (h % block_y);
			int j =  -part_block_y + (w % block_x);
			const float4 current_image_block = 255.0f * read_imagef(image, (int2)(w, h)) * norm * Gaussian_filter_xy(convert_float(i), convert_float(j), sigma);
			result_buffer[(h)* width  + (w)] = convert_uchar4_sat_rte(current_image_block);
		}
}
__kernel void make_gauss_vec1_image_float_rgba(__global float4* result_buffer, read_only image2d_t image, const int width,const int height, int block_x, int block_y){
    int idx = get_global_id(0);
    int idy = get_global_id(1);
	float w = 4.0f * sqrt(2.0f * log(2.0f));
	float sigma = block_x / w;
	float4 norm = block_x * block_y; 
	int part_block_x = (block_x - 1)/ 2; 
	int part_block_y = (block_y - 1) / 2; 
    int stride_x = get_global_size(0);
    int stride_y = get_global_size(1);
	for (int h = idy; h < height; h+=stride_y)
		for (int w = idx; w <  width; w+=stride_x){
			int i =  -part_block_y + (h % block_y);
			int j =  -part_block_y + (w % block_x);
			result_buffer[(h)* width  + (w)] = 255.0f * read_imagef(image, (int2)(w, h)) * norm * Gaussian_filter_xy(convert_float(i), convert_float(j), sigma);
		}
}
__kernel void version_1_MSE_stabilization_image_rgba(__global uchar4* result_buffer, read_only image2d_t image_current, read_only image2d_t image_next, const int width,const int height,  int radius, int block_x, int block_y){

	int local_id = get_local_id(0) + get_local_id(1) * get_local_size(0);
	int local_size = get_local_size(0)*get_local_size(1);
	int size_block = block_x * block_y;
	int number_blocks_in_local_memory = local_size / size_block;
	int index_block_in_local_memory = local_id / size_block;
	int h_border = height - block_y;
	int w_border = width - block_x;
	for (int y = get_global_id(1)*block_y; y< h_border; y+=get_global_size(1) ){
		for (int x = get_global_id(0)*block_x; x < w_border; x+=get_global_size(0)){	
			float min = FLT_MAX;
			int2 index = 0;
			for (int i = -radius; i <= radius; i++){
				for (int j = -radius; j <= radius; j++){
					float4 sum = 0.0f;
					for (int h = 0; h < block_y; h++){
						for (int w = 0; w < block_x; w++){
							const float4 next_image = 255.0f * read_imagef(image_next, (int2)(w+x +j, h+y+i));
							const float4 current_image = 255.0f * read_imagef(image_current, (int2)(w+x, h+y));
							const float4 sub_data = current_image - next_image;
							sum += sub_data * sub_data;
						}
					}
					sum = sum/(block_x*block_y);
					const float result = sum.x * sum.y * sum.z * sum.w;
			
					index = result < min ? (int2)(i, j) : index;
					min = result < min ? result : min;
				}	
			}
			//printf("%i %i  %i %i %f\n", x, y,index.x, index.y, min);
			//if (y<2&&x==170)
			//	printf("%i %i , %v2i\n", x, y,index);
			for (int h = index.y, _h=0; h <= block_y+index.y; h++, _h++){
				for (int w = index.x, _w=0; w <= block_x+index.x; w++, _w++){
					const float4 next_image = 255.0f * read_imagef(image_next, (int2)(w+x, h+y));
					result_buffer[(_h + y) * width + (_w + x)] = convert_uchar4_sat_rte(next_image);
				}
			}
		}
	}
}
__kernel void image_stabilization_vec1_image_rgba(__global uchar4* result_buffer, read_only image2d_t image_current, read_only image2d_t image_next, const int width_current,const int height_current,  int width_next, int height_next, int block_x, int block_y, __local float* local_data){

	int blocks_x =  get_local_id(0) / block_x; 
	int blocks_y =  get_local_id(1) / block_y; 
	int local_id = get_local_id(0) + get_local_id(1) * get_local_size(0);
	int local_stride = get_local_size(0) * get_local_size(1);
	int local_size = get_local_size(0)*get_local_size(1);
	int size_block = block_x * block_y;
	int number_blocks_in_local_memory = local_size / size_block;
	int index_block_in_local_memory = (blocks_y * get_local_size(0)/block_x + blocks_x);
	int size_local_block = convert_int(log2(convert_float(size_block)));
	int stride_x = get_global_size(0);
	int stride_y = get_global_size(1);
	
	for (int i = local_id; i < local_size; i+=local_stride)
			local_data[i] = 0.0f;

	float max = 0.0f;
    __local float* ptr_local = local_data + index_block_in_local_memory * size_block;
	for (int y = get_global_id(1); y < height_current; y+=stride_y){
		for (int x = get_global_id(0); x < width_current; x+=stride_x){
			int index_x = 0;
			int index_y = 0;
			int index_blocks_x =  get_local_id(0) % block_x; 
			int index_blocks_y =  get_local_id(1) % block_y; 
			int index_blocks = index_blocks_y * block_x + index_blocks_x;
			int best_index_x = index_blocks_x;
			int best_index_y = index_blocks_y;
			const float4 current_image_block = 255.0f * read_imagef(image_current, (int2)(x, y));
			for (int i = 0; i < height_next; i++){
				for (int j = 0; j < width_next; j++){
					int _i = index_blocks;
					int _j = size_block + 1 >> 1;
					index_x = j + index_blocks_x;
					index_y = i + index_blocks_y;
					const float4 next_image_block = 255.0f * read_imagef(image_next, (int2)(index_x, index_y));
					ptr_local[_i] = current_image_block.x * next_image_block.x;
					barrier(CLK_LOCAL_MEM_FENCE);
					int save_j = size_block;
					while ( save_j >= 2){
						const int offset =  _i + _j;
						if  (offset < save_j )
							ptr_local[_i] += ptr_local[offset];
						barrier(CLK_LOCAL_MEM_FENCE);
						save_j = _j ;
						_j =  (_j + 1) >> 1;
					}
					if (max < ptr_local[0]){
						max = ptr_local[0];
						best_index_x = index_x;
						best_index_y = index_y;
					}
						//if (x < 3 && y < 3)
						//	printf("%f %u %u %u %u\n", ptr_local[0], best_index_y, best_index_x, size_local_block, _j);
						//return;
				}
			}		
		//	if (x < 7 && y < 7)
		//		printf("%u %u %u %u %u %u %u\n",index_block_in_local_memory, _j , index_blocks_x,index_blocks_y, index_blocks,  best_index_x, best_index_y );
			result_buffer[y * width_current + x] = convert_uchar4_sat_rte(255.0f * read_imagef(image_next, (int2)(best_index_x, best_index_y))); 
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