#include "fractalImageCompression.h"
void fractalImageCompression::writeImage(cl_uint2* position_xy, cl_float* _q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, char* nameFile, size_t width, size_t height) {
	std::ofstream file(nameFile, std::ios_base::out | std::ios_base::trunc);
	file << "width" << "\t" << width << "\t" << "height" << "\t" << height << "\n";
	file << "domain" << "\t" << size_domain << "\t" << "rank" << "\t" << size_rank << "\n";
	for (size_t i = 0; i < height / size_rank; i++) {
		for (size_t j = 0; j < width / size_rank; j++) {
			file << position_xy->s0 << "\t" << position_xy->s1 << "\t" << (int)*typeFigure++ << "\t" << *_q++ << '\n';
			position_xy++;
		}
	}

	file.close();
}
void fractalImageCompression::compress(cl_uint2* errors, cl_uchar* image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height) {
	size_t index = 0;

	const int idx = 0;
	const int idy = 0;
	const size_t size_div_dom_rank = size_domain / size_rank;
	const int number_rank_blocks_x = width / size_rank;
	const int number_rank_blocks_y = height / size_rank;
	float mean_1 = 0;
	float mean_2 = 0;
	float medium = 0;
	float disp = 0;
	float min_disp = CL_FLT_MAX;
	const float transformation_plus_scale[] = {
		1,0,0,1,
		0,1,-1,0,
		-1,0,0,-1,
		0,-1,1,0,
		0,-1,-1,0,
		-1,0,0,1,
		0,1,1,0,
		1,0,0,-1,
		float(size_rank) / float(size_domain), 0, 0, float(size_rank) / float(size_domain) };
	const float size_block = size_domain * size_domain;
	float best_figure = 0;
	float best_q = 0;
	float best_position_x = 0;
	float best_position_y = 0;
	int width_without_memory_leak = width - width % size_domain;
	int height_without_memory_leak = height - height % size_domain;
	//#pragma omp parallel for private(best_figure, best_q,best_position_x, best_position_y,disp,medium,mean_2,mean_1 )

	for (int i = idy; i < number_rank_blocks_y; i += 1) {
		for (int j = idx; j < number_rank_blocks_x; j += 1) {
			const float start_rank_x = j * size_rank;
			const float start_rank_y = i * size_rank;
			for (int h = 0; h < height_without_memory_leak; h += size_domain) {
				for (int w = 0; w < width_without_memory_leak; w += size_domain) {
					for (int index_transformation = 0; index_transformation < 32; index_transformation += 4) {
						float matrix_00 = transformation_plus_scale[32] * transformation_plus_scale[index_transformation] + transformation_plus_scale[32 + 1] * transformation_plus_scale[index_transformation + 2];
						float matrix_01 = transformation_plus_scale[32] * transformation_plus_scale[index_transformation + 1] + transformation_plus_scale[32 + 1] * transformation_plus_scale[index_transformation + 3];
						float matrix_10 = transformation_plus_scale[32 + 2] * transformation_plus_scale[index_transformation] + transformation_plus_scale[32 + 3] * transformation_plus_scale[index_transformation + 2];
						float matrix_11 = transformation_plus_scale[32 + 2] * transformation_plus_scale[index_transformation + 1] + transformation_plus_scale[32 + 3] * transformation_plus_scale[index_transformation + 3];
						for (int b_y = 0; b_y < size_domain; b_y++) {
							for (int b_x = 0; b_x < size_domain; b_x++) {
								float x_index = (b_x + 10e-7f);
								float y_index = (b_y + 10e-7f);
								float _x_index = x_index * matrix_00 + y_index * matrix_01;
								float _y_index = x_index * matrix_10 + y_index * matrix_11;
								_y_index = _y_index < 0 ? (size_rank)+_y_index : _y_index;
								_x_index = _x_index < 0 ? (size_rank)+_x_index : _x_index;

								const float data_dom_block = image[(h + b_y) * width + b_x + w];
								const float data_rank_block = image[int(_x_index + start_rank_x) + int(_y_index + start_rank_y) * width];
								medium += data_rank_block - data_dom_block;
								mean_1 += data_rank_block;
								mean_2 += data_dom_block;
							}
						}
						medium /= size_block;
						for (int b_y = 0; b_y < size_domain; b_y++) {
							for (int b_x = 0; b_x < size_domain; b_x++) {
								float x_index = (b_x + 10e-7f);
								float y_index = (b_y + 10e-7f);
								float _x_index = x_index * matrix_00 + y_index * matrix_01;
								float _y_index = x_index * matrix_10 + y_index * matrix_11;
								_y_index = _y_index < 0 ? (size_rank)+_y_index : _y_index;
								_x_index = _x_index < 0 ? (size_rank)+_x_index : _x_index;
								const float data_dom_block = image[(h + b_y) * width + (b_x + w)];
								const float data_rank_block = image[int(_x_index + start_rank_x) + int(_y_index + start_rank_y) * width];
								const float part_disp_rank_dom = data_rank_block - data_dom_block - medium;
								disp += part_disp_rank_dom * part_disp_rank_dom;
							}
						}
						disp /= size_block;
						if (disp < min_disp) {
							min_disp = disp;
							best_figure = index_transformation;
							best_q = abs(mean_2 - mean_1);
							best_position_x = w;
							best_position_y = h;
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
			q[index_block] = best_q / size_block;
			typeFigure[index_block] = best_figure / 4;

			//float matrix_00 = transformation_plus_scale[32] * transformation_plus_scale[int(best_figure)] + transformation_plus_scale[32 + 1] * transformation_plus_scale[int(best_figure) + 2];
			//float matrix_01 = transformation_plus_scale[32] * transformation_plus_scale[int(best_figure) + 1] + transformation_plus_scale[32 + 1] * transformation_plus_scale[int(best_figure) + 3];
			//float matrix_10 = transformation_plus_scale[32 + 2] * transformation_plus_scale[int(best_figure)] + transformation_plus_scale[32 + 3] * transformation_plus_scale[int(best_figure) + 2];
			//float matrix_11 = transformation_plus_scale[32 + 2] * transformation_plus_scale[int(best_figure) + 1] + transformation_plus_scale[32 + 3] * transformation_plus_scale[int(best_figure) + 3];
			//for (int b_y = 0; b_y < size_domain; b_y++) {
			//	for (int b_x = 0; b_x < size_domain; b_x++) {
			//		float x_index = (b_x + 10e-7f);
			//		float y_index = (b_y + 10e-7f);
			//		float _x_index = x_index * matrix_00 + y_index * matrix_01;
			//		float _y_index = x_index * matrix_10 + y_index * matrix_11;
			//		_y_index = _y_index < 0 ? (size_rank)+_y_index : _y_index;
			//		_x_index = _x_index < 0 ? (size_rank)+_x_index : _x_index;
			//		errors[index].x = int(_x_index);
			//		errors[index++].y = int(_y_index);
			////		printf("%u%u ", int(_x_index), int(_y_index));
			//	}
			//	//printf("\n");
			//}

			position_xy[index_block].x = best_position_x / size_domain;
			position_xy[index_block].y = best_position_y / size_domain;
		}
	}

	index = 0;
}

void fractalImageCompression::decompress(cl_uint2* errors, cl_uchar* image, cl_uchar* result_image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height) {
	size_t index = 0;
	const float transformation_plus_scale[] = {
		1,0,0,1,
		0,1,-1,0,
		-1,0,0,-1,
		0,-1,1,0,
		0,-1,-1,0,
		-1,0,0,1,
		0,1,1,0,
		1,0,0,-1,
		 float(size_rank) / float(size_domain), 0, 0, float(size_rank) / float(size_domain) };
	int rank_blocks_x = width / size_rank;
	int rank_blocks_y = height / size_rank;
	//for (size_t i = 0; i < rank_blocks_x*rank_blocks_y; i++)
	//	printf("%u   %u %u\n", i, position_xy[i].x, position_xy[i].y);
	int size_div_dom_rank = size_domain / size_rank;
	float sum_dom = 0;
	for (int i = 0; i < rank_blocks_y; i++) {
		for (int j = 0; j < rank_blocks_x; j++) {
			int start_rank_x = j * size_rank;
			int start_rank_y = i * size_rank;
			const int index_block = i * rank_blocks_x + j;
			const int index_dom = (size_domain *position_xy[index_block].y*width + size_domain * position_xy[index_block].x);
			cl_uchar type = typeFigure[index_block] * 4;
			cl_float shift = q[index_block];
			float matrix_00 = transformation_plus_scale[type];
			float matrix_01 = transformation_plus_scale[type + 1];
			float matrix_10 = transformation_plus_scale[type + 2];
			float matrix_11 = transformation_plus_scale[type + 3];
			for (int b_y = 0; b_y < size_domain; b_y++) {
				for (int b_x = 0; b_x < size_domain; b_x++) {
					sum_dom += image[index_dom + (b_y)* width + b_x];
				}
			}
			for (int b_y = 0; b_y < size_domain; b_y += size_div_dom_rank) {
				for (int b_x = 0; b_x < size_domain; b_x += size_div_dom_rank) {
					float sum_block_dom = 0;
					for (int block_y = 0; block_y < size_div_dom_rank; block_y++) {
						for (int block_x = 0; block_x < size_div_dom_rank; block_x++) {
							float x_index = (b_x + block_x + 10e-7f);
							float y_index = (b_y + block_y + 10e-7f);
							float _x_index = x_index * matrix_00 + y_index * matrix_01;
							float _y_index = x_index * matrix_10 + y_index * matrix_11;
							_y_index = _y_index < 0 ? (size_domain)+_y_index : _y_index;
							_x_index = _x_index < 0 ? (size_domain)+_x_index : _x_index;
							const float data_dom_block = image[index_dom + int(_y_index)* width + int(_x_index)];
							sum_block_dom += data_dom_block;
							//		printf("%u%u ", int(_y_index), int(_x_index));
						}
						//		printf("\n");
					}
					//	printf("\n");
					sum_block_dom /= (size_div_dom_rank*size_div_dom_rank);
					result_image[start_rank_x + (start_rank_y)* width] = (sum_block_dom)+shift;
					start_rank_x++;
				}
				start_rank_x = j * size_rank;
				start_rank_y++;
			}

			//for (int b_y = 0; b_y < size_domain; b_y++) {
			//	for (int b_x = 0; b_x < size_domain; b_x++) {
			//		float x_index = (b_x + 10e-7f);
			//		float y_index = (b_y + 10e-7f);
			//		float _x_index = x_index * matrix_00 + y_index * matrix_01;
			//		float _y_index = x_index * matrix_10 + y_index * matrix_11;
			//		_y_index = _y_index < 0 ? (size_domain)+_y_index : _y_index;
			//		_x_index = _x_index < 0 ? (size_domain)+_x_index : _x_index;
			//		if (errors[index].x != int(_x_index / 4) || errors[index++].y != int(_y_index / 4)) {
			//			printf("%u%u%u ", index, int(_x_index / 4) , int(_y_index / 4));
			//		}
			//	//	printf("%u%u ", int(_x_index / 4), int(_y_index / 4));
			//	}
			////	printf("\n");
			//}
		}
	}
}
void fractalImageCompression::compress_OpenCL(clDevice* device, cl_uchar* image, cl_uint2* position_xy, cl_float* q, cl_uchar* typeFigure, size_t size_domain, size_t size_rank, size_t width, size_t height) {
}

fractalImageCompression::fractalImageCompression(clDevice* device, cl_uchar4* image, size_t width, size_t height, size_t size_domain, size_t size_rank)
{
	const int number_rank_blocks_x = width / size_rank;
	const int number_rank_blocks_y = height / size_rank;
	cl_uchar* R = (cl_uchar*)malloc(2 * width*height);
	cl_uchar* G = (cl_uchar*)malloc(2 * width*height);
	cl_uchar* B = (cl_uchar*)malloc(2 * width*height);

	cl_uint2* errors = (cl_uint2*)malloc(4 * size_rank * size_rank * width*height * sizeof(cl_uint2));

	cl_uchar* A = (cl_uchar*)malloc(width*height);
	size_t offset = number_rank_blocks_x * number_rank_blocks_y;
	cl_uint2* position_xy = (cl_uint2*)malloc(4 * number_rank_blocks_x*number_rank_blocks_y * sizeof(cl_uint2));
	cl_float* shift = (cl_float*)malloc(4 * number_rank_blocks_x*number_rank_blocks_y * sizeof(cl_float));
	cl_uchar* typeFigure = (cl_uchar*)malloc(4 * number_rank_blocks_x*number_rank_blocks_y * sizeof(cl_uchar));
	for (size_t i = 0; i < width*height; i++) {
		R[i] = image[i].s0;
		G[i] = image[i].s1;
		B[i] = image[i].s2;
	}
	compress(errors, R, position_xy, shift, typeFigure, size_domain, size_rank, width, height);
	compress(errors + size_rank * size_rank * width*height, G, position_xy + offset, shift + offset, typeFigure + offset, size_domain, size_rank, width, height);
	compress(errors + size_rank * size_rank * width*height * 2, B, position_xy + offset * 2, shift + offset * 2, typeFigure + offset * 2, size_domain, size_rank, width, height);
	for (size_t i = 0; i < width*height; i++) {
		R[i] = 0;
		G[i] = 0;
		B[i] = 0;
	}
	writeImage(position_xy, shift, typeFigure, size_domain, size_rank, "output.txt", width, height * 3);
	int i = 0;
	for (; i < 35; i++) {
		decompress(errors + size_rank * size_rank * width*height * 0, R + (i % 2)* width*height, R + ((i % 2) ^ 1)* width*height, position_xy, shift, typeFigure, size_domain, size_rank, width, height);
		decompress(errors + size_rank * size_rank * width*height * 1, G + (i % 2)* width*height, G + ((i % 2) ^ 1)* width*height, position_xy + offset, shift + offset, typeFigure + offset, size_domain, size_rank, width, height);
		decompress(errors + size_rank * size_rank * width*height * 2, B + (i % 2)* width*height, B + ((i % 2) ^ 1)* width*height, position_xy + offset * 2, shift + offset * 2, typeFigure + offset * 2, size_domain, size_rank, width, height);
	}
	cl_uchar* ptr_R = R + ((i % 2))* width*height;
	cl_uchar* ptr_G = G + ((i % 2))* width*height;
	cl_uchar* ptr_B = B + ((i % 2))* width*height;
	for (size_t i = 0; i < width*height; i++) {
		image[i].s0 = ptr_R[i];
		image[i].s1 = ptr_G[i];
		image[i].s2 = ptr_B[i];
	}
	free(R);
	free(G);
	free(B);
	free(A);
	free(position_xy);
	free(shift);
	free(typeFigure);
	free(errors);
}

fractalImageCompression::~fractalImageCompression()
{
}