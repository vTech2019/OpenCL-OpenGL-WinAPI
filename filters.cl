R"===(

__kernel void inverse_matrix_Gaussian_filter_float(read_only image2d_t image, write_only image2d_t image_write, const int width, const int height){

    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	int2 coord_xy;
    float rad = (3.1415926535f / 180.0f) * 180.0f;
	float2 center_rotate_xy;

	for (int h = idy; h < height ; h += stride_y){
		for (int w = idx ; w <  width ; w += stride_x){
			float4 data_image = read_imagef(image, (int2)(w, h));
			if ( w < width / 2 && h < height / 2){
				 center_rotate_xy = (float2)(width / 4, height / 4);
			}else if (w > width / 2 && h < height / 2){
				 center_rotate_xy = (float2)(3 * width / 4, height / 4);
			}else if (w < width / 2 && h > height / 2){
				 center_rotate_xy = (float2)(width / 4,3 * height / 4);
			}else if (w > width / 2 && h > height / 2){
				 center_rotate_xy = (float2)(3 * width / 4,3 * height / 4);
			}
			coord_xy.x = center_rotate_xy.x + (w - center_rotate_xy.x) * native_cos(rad) - (h - center_rotate_xy.y) * native_sin(rad);
			coord_xy.y = center_rotate_xy.y + (w - center_rotate_xy.x) * native_sin(rad) + (h - center_rotate_xy.y) * native_cos(rad);

			write_imagef(image_write, coord_xy, data_image);
		}
	}
}

__kernel void sum_float4_image_rgba(read_only image2d_t image, write_only image2d_t  sums, const int width,const int height, const int firstCall, const int true_size_image,const int mean, __local float4* reductionSums)
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	const int local_idx = get_local_id(0);
	const int local_idy = get_local_id(1);
	const int local_size_x = get_local_size(0);
	const int local_size_y = get_local_size(1);
	const int get_group_id_x =  get_group_id (0);
	const int get_group_id_y =  get_group_id (1);
	const int all_id_local = local_idy * local_size_x + local_idx;
	if (idx < width && idy < height){
		if (firstCall == true)
			reductionSums[all_id_local] = 255.0f  * read_imagef(image, (int2)(idx, idy));
		else
			reductionSums[all_id_local] = read_imagef(image, (int2)(idx, idy));
		for (int stride = local_size_y * local_size_x / 2; stride>0; stride /=2){
			barrier(CLK_LOCAL_MEM_FENCE);
			if (all_id_local < stride ){
				reductionSums[all_id_local] += reductionSums[all_id_local + stride];
			}
		}
		if (all_id_local == 0){
			//printf("%u\n", get_num_groups(0) + get_num_groups(1));
			if (get_num_groups(0) == 1 && get_num_groups(1) == 1 && mean)
				reductionSums[0]/=true_size_image;
			write_imagef(sums, (int2)(get_group_id_x ,  get_group_id_y), (reductionSums[0]));
		}
	}

}



__kernel void add_image_rgba_image(read_only image2d_t image_read_1, read_only image2d_t image_read_2, write_only image2d_t image_write, const int width, const int height )
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	for (int i = idy; i < height; i+=stride_y) {
		for (int j = idx; j < width; j+=stride_x) {
			float4 data_image_1 = read_imagef(image_read_1, (int2)(j, i));
			float4 data_image_2 = read_imagef(image_read_2, (int2)(j, i));
			
			float4 new_image = data_image_1 + data_image_2;
			write_imagef(image_write, (int2)(j ,  i), new_image );
			
		}
	}
}

__kernel void div_image_rgba_image(read_only image2d_t image_read_1, read_only image2d_t image_read_2, write_only image2d_t image_write, const int width, const int height )
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	for (int i = idy; i < height; i+=stride_y) {
		for (int j = idx; j < width; j+=stride_x) {
			float4 data_image_1 = read_imagef(image_read_1, (int2)(j, i));
			float4 data_image_2 = read_imagef(image_read_2, (int2)(j, i));
			
			float4 new_image = data_image_1 / data_image_2;
			write_imagef(image_write, (int2)(j ,  i), new_image );
			
		}
	}
}
__kernel void div_number_image_rgba_image(read_only image2d_t image_read_1, read_only image2d_t image_read_2, write_only image2d_t image_write, const int width, const int height )
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	float4 data_image_2 = read_imagef(image_read_2, (int2)(0, 0));
	for (int i = idy; i < height; i+=stride_y) {
		for (int j = idx; j < width; j+=stride_x) {
			float4 data_image_1 = read_imagef(image_read_1, (int2)(j, i));
			
			float4 new_image = data_image_1 / data_image_2;
			write_imagef(image_write, (int2)(j ,  i), new_image );
			
		}
	}
}

__kernel void mul_number_image_rgba_image(read_only image2d_t image_read_1, read_only image2d_t image_read_2, write_only image2d_t image_write, const int width, const int height )
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	float4 data_image_2 = read_imagef(image_read_2, (int2)(0, 0));
	for (int i = idy; i < height; i+=stride_y) {
		for (int j = idx; j < width; j+=stride_x) {
			float4 data_image_1 = read_imagef(image_read_1, (int2)(j, i));
			
			float4 new_image = data_image_1 * data_image_2;
			write_imagef(image_write, (int2)(j ,  i), new_image );
			
		}
	}
}

__kernel void mul_image_rgba_image(read_only image2d_t image_read_1, read_only image2d_t image_read_2, write_only image2d_t image_write, const int width, const int height )
{
    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	for (int i = idy; i < height; i+=stride_y) {
		for (int j = idx; j < width; j+=stride_x) {
			float4 data_image_1 = read_imagef(image_read_1, (int2)(j, i));
			float4 data_image_2 = read_imagef(image_read_2, (int2)(j, i));
			
			float4 new_image = data_image_1 * data_image_2;
			write_imagef(image_write, (int2)(j ,  i), new_image );
			
		}
	}
}

__kernel void winner_approximation_rgba(read_only image2d_t Re_image,read_only image2d_t Im_image,read_only image2d_t Re_kernel,read_only image2d_t Im_kernel,write_only image2d_t Re_write,write_only image2d_t Im_write, const float K, const int width, const int height){

    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	for (int h = idy; h < height ; h += stride_y){
		for (int w = idx ; w <  width ; w += stride_x){
			float4 image_Re = read_imagef(Re_image, (int2)(w, h));
			float4 image_Im = read_imagef(Im_image, (int2)(w, h));
			float4 kernel_Re = read_imagef(Re_kernel, (int2)(w, h));
			float4 kernel_Im = read_imagef(Im_kernel, (int2)(w, h));
			float4 F_Re = ((image_Re)/(image_Re*image_Re + K)) * kernel_Re;
			float4 F_Im = ((image_Im)/(image_Im*image_Im + K)) * kernel_Im;
			write_imagef(Re_write, (int2)(w,h), F_Re);
			write_imagef(Im_write, (int2)(w,h), F_Im);
		}
	}
}




)==="