R"===(


/*__kernel void fractal_float_image_rgba(read_only image2d_t image, write_only image2d_t  compress_information, const int width,const int height, const int size_rank, const int size_domain){


    const int idx = get_global_id(0);
    const int idy = get_global_id(1);
    const int stride_x = get_global_size(0);
    const int stride_y = get_global_size(1);
	const size_t size_div_dom_rank = size_domain / size_rank;
	const int number_rank_blocks_x = width / size_rank;
	const int number_rank_blocks_y = height / size_rank;
	float4 mean_1 = 0;
	float4 mean_2 = 0;
	float4 medium = 0;
	float4 disp = 0;
	float min_disp = CL_FLT_MAX;
	const float transformation_plus_scale[] = { 1,0,0,1,  0,1,-1,0, -1,0,0,-1, 0,-1,1,0, 0,-1,-1,0, -1,0,0,1, 0,1,1,0, 1,0,0,-1, float(size_rank) / float(size_domain), 0, 0, float(size_rank) / float(size_domain) };
	const float size_block = size_domain * size_domain;
	for (int i = idy; i < number_rank_blocks_y; i += stride_y) {
		for (int j = idx; j < number_rank_blocks_x; j += stride_x) {
				const float start_rank_x = j * size_rank;
			const float start_rank_y = i * size_rank;
			for (int h = 0; h < height; h += size_domain) {
				for (int w = 0; w < width; w += size_domain) {
					for (int index_transformation = 0; index_transformation < 32; index_transformation += 4) {
						for (int b_y = 0; b_y < size_domain; b_y++) {
							for (int b_x = 0; b_x < size_domain; b_x++) {
								int4 x_index = (start_rank_x + size_div_dom_rank);
								int4 y_index = (start_rank_y + size_div_dom_rank);
								float index_image_x = transformation_plus_scale[32 + 3] * transformation_plus_scale[index_transformation] + transformation_plus_scale[32 + 0] * transformation_plus_scale[index_transformation + 1];
								float index_image_y = transformation_plus_scale[32 + 0] * transformation_plus_scale[index_transformation + 3] + transformation_plus_scale[32 + 3] * transformation_plus_scale[index_transformation + 2];
								x_index *= index_image_x;
								y_index *= index_image_y;
								y_index = y_index < 0 ? (size_rank)+y_index : y_index == 0 ? y_index : y_index - 1;
								x_index = x_index < 0 ? (size_rank)+x_index : x_index == 0 ? x_index : x_index - 1;
								y_index += h;
								x_index += w;
								const float4 data_dom_block = read_imagef(image, (int2)(b_x + w, h + b_y));
								const float4 data_rank_block = read_imagef(image, (int2)(x_index, y_index));
								medium += data_rank_block - data_dom_block;
								mean_1 += data_rank_block;
								mean_2 += data_dom_block;
								//printf("%u ", x_index + y_index * width);
							}
							//printf("\n");
						}
						medium /= size_block;
						for (int b_y = 0; b_y < size_domain; b_y++) {
							for (int b_x = 0; b_x < size_domain; b_x++) {
								int index_image = (start_rank_x + b_x) * transformation_plus_scale[32 + 3];
								index_image += (start_rank_y + b_y) * transformation_plus_scale[32 + 0] * width;
								const float data_dom_block = image[(h + b_y) * width + (b_x + w)];
								const float data_rank_block = image[index_image];
								const float part_disp_rank_dom = data_rank_block - data_dom_block - medium;
								disp += part_disp_rank_dom * part_disp_rank_dom;
							}
						}
						disp /= size_block;
						if (disp < min_disp) {
							min_disp = disp;
							best_figure = index_transformation / 4;
							best_q = (mean_2 - mean_1) / size_block;
							best_position_x = w / size_domain;
							best_position_y = h / size_domain;
						}
						disp = 0;
						medium = 0;
					}
					mean_2 = 0;
					mean_1 = 0;
				}
			}
			min_disp = FLT_MAX;
			const int index_block = i * number_rank_blocks_x + j;
			q[index_block] = best_q;
			typeFigure[index_block] = best_figure;
			position_xy[index_block].x = best_position_x;
			position_xy[index_block].y = best_position_y;
		}
	}



}

*/





)==="