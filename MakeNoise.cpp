#include "MakeNoise.h"

cl_float4* MakeNoise::inverse_matrix(cl_float4* matrix, size_t width, size_t height) {
	//float det = 0.0f;
//int l;
//double determinant;
//double sum11 = 1, sum12 = 0, sum21 = 1, sum22 = 0;
//for (int i = 0; i < height; i++) {
//	sum11 = 1;
//	l = 2 * width - 1 - i;
//	sum21 = 1;
//	for (int j = 0; j < width; j++) {
//		sum21 *= kernel[j * width + l % width].x;
//		l--;
//		sum11 *= kernel[j*width + (j + i) % (width)].x;
//	}
//	sum22 += sum21;
//	sum12 += sum11;
//}
//determinant = sum12 - sum22;
//size_t number_matrix = 0;
//height = 4;
//for (size_t i = 0, j = height; i < height - 2; i++, j--)
//	number_matrix += j * j;
//cl_float** A = (cl_float**)calloc(number_matrix, sizeof(cl_float*));
//for (size_t level = 0, k = 0, size_matrix = height; level < height - 2; level++, size_matrix--)
//	for (size_t level_matrix = 0; level_matrix < size_matrix *size_matrix; level_matrix++)
//		A[k++] = (cl_float*)calloc((size_matrix - 1)*(size_matrix - 1), sizeof(cl_float));
//if (determinant) {
//	for (int h = 0; h < height; h++) {
//		for (int w = 0; w < width; w++) {
//			for (size_t level = 0, k = 0, size_matrix = height; level < height - 2; level++, size_matrix--)
//				for (size_t level_matrix = 0, width_matrix = size_matrix - 1; level_matrix < size_matrix * size_matrix; level_matrix++, k++)
//					for (size_t i = 0; i < width_matrix; i++) {
//						for (size_t j = 0; j < width_matrix; j++) {
//							A[k][i*width + j] = ;......................................
//							.................................................
//					}
//		}
//	}
//}

//sum /= width * height;
//for (size_t i = 0; i < number_matrix; i++)
//	free(A[i]);
//free(A);
	cl_float stack_inverse_matrix[] = { 1079.204236,-1280.472923,563.3703743,-542.8517522,312.5876733,-95.0592449,301.4978835,-211.8201197,281.8757206,-389.8347008,221.2658148,-405.8619612,272.0123509,236.2220432,-200.0677839,562.6148751,-1014.939339,469.5304903,-350.7180398,461.2502299,
	-1741.680486,2465.283744,-1560.473901,1118.867537,-543.6481234,241.679635,-356.8918998,121.2072465,-254.6067705,312.6995557,-74.23812947,421.6264561,-136.2535866,-664.0275573,631.7840577,-1177.430812,1512.586803,-50.28616707,-481.9812649,-125.5425411,
	1235.062476,-2069.109696,1425.775599,-743.3189235,414.2624663,-259.4659574,160.4442338,-17.82619182,-21.37002428,177.5136894,-204.4152734,-100.0481793,-133.5368396,648.3109903,-712.0453605,964.9728375,-764.0722648,-640.5828711,1258.526129,-482.6100411,
	-1083.307174,1566.603028,-791.8085298,473.0567971,-404.6270468,197.012472,-221.4218539,184.1278204,-12.80715277,-123.9538528,65.61963331,201.9323653,-43.14041207,-429.7751132,572.5492647,-724.2075498,586.5923481,224.6492014,-477.4470095,85.07443892,
	714.4461149,-1036.178588,725.475866,-606.2874015,361.5562962,-158.0187077,145.811338,6.083659134,-65.39646374,90.69674937,-96.2723563,-109.6507749,-29.48988957,388.5576687,-458.7089734,665.1946242,-557.9156911,-44.75880648,-45.37147578,271.7418068,
	166.0068711,-87.50403434,-167.5732328,126.4245916,-79.67756125,109.0096442,37.17219645,-124.4258923,98.92502244,-145.9562644,142.2613916,-121.8579795,158.3132045,-63.88579245,5.729191029,-102.1809716,22.37030146,2.551510519,259.6597025,-236.1131723,
	-265.2408814,458.9930538,-353.2741233,170.4581942,-8.238208323,-42.08553079,39.71609627,-96.01744907,114.5378242,-96.49631462,63.00060073,-7.05743752,74.79259633,-206.0347004,256.0892219,-272.1180311,47.02565276,415.0765864,-501.2981351,179.6290484,
	-129.0915029,87.10705089,-62.21012995,154.994926,-3.162120707,-60.385749,-67.41232975,-25.16981033,101.6187148,-36.50540042,81.66997049,-49.82750466,-38.74682954,-49.84318794,62.61460863,22.3671184,13.98882927,-7.750087444,-40.43668388,9.258130801,
	369.7567069,-425.0119456,173.5063553,-84.97499273,-173.5630925,216.3673688,34.46867273,174.4505773,-450.9330505,332.8932144,-278.2204878,247.9861884,-125.5678421,184.0351214,-241.7695806,105.6778004,166.137286,-466.6018204,406.9066621,-95.7799318,
	-592.6254386,528.5394887,66.53369489,-76.18510707,245.618013,-313.1001604,-103.3438019,-47.58699476,375.8235268,-190.4949902,181.5183472,-228.9198467,12.30538546,-86.58138464,229.1108594,-75.00006815,0.801594868,68.13302468,-200.1552744,112.6596053,
	601.3396809,-572.8963996,105.5574959,-147.3488733,-131.9195769,283.3511424,66.65251332,102.4129614,-319.0453643,153.4833505,-281.4723206,283.0819883,-19.73044523,167.9117952,-300.4072577,153.6488032,-125.1786094,10.22317069,108.3388968,-21.44425519,
	-598.078065,625.6749929,-225.4411529,255.737933,31.78777471,-211.8494573,-52.47065867,-98.7867911,284.4919212,-206.6552682,311.0767584,-244.396221,71.49146151,-193.7397972,254.345894,-229.8953853,78.71238366,257.3100178,-126.0310274,-128.2799993,
	570.9042872,-603.1366038,181.9818808,-204.143766,19.59447722,127.5525241,126.7411873,-70.86410656,-83.49222805,13.19252007,-51.35663004,22.88441565,50.88011219,86.46157044,-177.6489084,260.0309931,-263.1997603,-193.9372194,506.625154,-229.913907,
	38.81325814,-125.246743,141.7321075,-107.9829829,112.5347025,-52.6179184,-37.54026734,-8.915705128,62.56522189,-24.97416988,48.27012349,-56.28840583,-43.96369133,53.76163098,-5.659609912,13.2948679,19.15562598,138.0621028,-444.9522785,307.5441616,
	-498.0412051,859.4603051,-683.6150983,517.5755286,-344.579458,110.1308557,-15.21627824,23.403548,-67.64024831,64.64071214,-69.58628082,140.6636649,6.761757784,-257.4099127,279.7298601,-395.2620412,484.6636824,-6.699957174,-420.1544453,226.1125357,
	620.6994271,-1022.099444,692.1400548,-427.2041985,329.903217,-197.2355875,29.79140145,112.9120173,-113.6970474,154.4068173,-153.9791064,-39.96465948,-41.51440432,289.7732827,-319.7021721,497.9184741,-421.3787045,-254.7792465,416.9685181,-58.11410423,
	-1153.936873,1453.055083,-572.0456147,333.4793544,-192.2369993,101.0336757,-281.9547321,28.74000613,162.1952141,-38.9361151,70.60490373,15.52080716,-41.11208738,-239.5167127,453.9824326,-637.7179216,354.8992158,431.9607415,-438.1719307,-10.29335759,
	420.1445039,-285.2542921,-192.6695732,11.44500431,89.41483423,-45.09183192,247.7106393,-115.415402,-21.87449126,-276.1329372,295.0570002,-27.27273703,-43.59700502,103.6266815,-142.5919117,65.84659018,86.34259322,-530.5285938,806.1357184,-391.6518446,
	1558.154294,-2578.229318,1944.855583,-1050.513809,87.93104608,174.2221947,189.8012629,-40.49765972,-139.74524,218.4047189,-419.7893071,30.65776959,271.5686727,348.6765162,-979.1816984,1290.688567,-1188.784194,190.1188559,604.629816,-344.2126099,
	-1348.132206,2203.178142,-1636.752885,939.7567873,-118.7520852,-144.673459,-225.4331399,56.27494943,135.909263,-46.81277688,215.6018925,-47.18112534,-178.437806,-383.8906195,926.1899168,-1094.068222,978.7446763,144.2176338,-1198.894596,710.5362228 };
	cl_float4* inverse_matrix = (cl_float4*)malloc(width * height * sizeof(cl_float4));
	cl_double sum = 0.0;
	for (size_t i = 0; i < width * height; i++) {
		sum += (stack_inverse_matrix[i]);
	}
	sum = (width * height) / sum;

	for (size_t i = 0; i < width * height; i++) {
		inverse_matrix[i].x = stack_inverse_matrix[i] * sum;
		inverse_matrix[i].y = stack_inverse_matrix[i] * sum;
		inverse_matrix[i].z = stack_inverse_matrix[i] * sum;
		inverse_matrix[i].w = stack_inverse_matrix[i] * sum;
	}
	return inverse_matrix;
}
cl_float4* MakeNoise::make_kernel_normal_distribution(size_t width, size_t height) {
	cl_float4* kernel = (cl_float4*)malloc(width * height * sizeof(cl_float4));
	float math_1 = 0;
	float math_2 = 0;
	float D_1 = 0;
	float D_2 = 0;
	float correlation = 0;
	float x_step = 1.0f / width;
	float y_step = 1.0f / height;
	for (size_t y = 0; y < height; y++)
		math_2 += y * y_step;
	for (size_t x = 0; x < width; x++)
		math_1 += x * x_step;
	math_1 /= width;
	math_2 /= height;
	for (size_t x = 0; x < width; x++)
		D_1 += pow(x * x_step - math_1, 2);
	for (size_t y = 0; y < height; y++)
		D_2 += pow(y * y_step - math_2, 2);
	float sum_x = D_1;
	float sum_y = D_2;
	D_1 /= width;
	D_2 /= height;
	float div = 0;
	for (size_t x = 0; x < width; x++)
		correlation += ((x * x_step - math_1) * (x * y_step - math_2));
	div = sqrt(sum_x * sum_y);
	correlation /= div;
	if (correlation == 1)
		correlation -= 0.000001f;
	float C = 1.0f / (2.0f * CL_M_PI * D_1 * D_2 * sqrtf(1.0f - correlation * correlation));
	cl_double sum = 0;
	cl_float max = 0;
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {
			float Q = (1.0f / (2.0f * 1.0f - correlation * correlation)) * ((powf(x * x_step - math_1, 2) / D_1) + (powf(y * y_step - math_2, 2) / D_2) + (2.0f * correlation * (x * x_step - math_1) * (y * y_step - math_2) / D_1 * D_2));
			cl_float data = C * expf(-Q);
			sum += data;
			kernel[y * width + x] = cl_float4{ data, data, data, data };
		}
	}
	sum = height * width / sum;
	for (size_t i = 0; i < height * width; i++) {
		kernel[i].x *= sum;
		kernel[i].y *= sum;
		kernel[i].z *= sum;
		kernel[i].w *= sum;
	}

	return kernel;
}
cl_float4* MakeNoise::getKernel() {
	return kernel;
}
MakeNoise::MakeNoise(clDevice* device, cl_uchar4* image, size_t width, size_t height, size_t width_filter, size_t height_filter)
{
	const void* null_ptr = NULL;
	cl_uchar type_arguments[] = { sizeof(cl_uint) };
	size_t length_row_pitch_data[] = { width * sizeof(cl_float4), width * sizeof(cl_uchar4), width_filter * sizeof(cl_float4), width_filter * sizeof(cl_uchar4) };
	kernel = make_kernel_normal_distribution(width_filter, height_filter);
	cl_int convolution_kernel_index = device->findKernel((const cl_char*)"convolution_image_float4_rgba", sizeof("convolution_image_float4_rgba"));
	cl_int convolution_normal_coord__kernel_index = device->findKernel((const cl_char*)"convolution_f_image_rgba", sizeof("convolution_f_image_rgba"));
	cl_int noise_kernel_index = device->findKernel((const cl_char*)"noise_image_rgba", sizeof("noise_image_rgba"));
	cl_uint kernel_gpu = device->mallocImageMemory((const void*)kernel, height_filter, width_filter, length_row_pitch_data[2], CL_RGBA, CL_FLOAT);
	cl_uint image_gpu = device->mallocImageMemory((const void*)image, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);
	cl_uint result_image_gpu = device->mallocImageMemory(null_ptr, height, width, length_row_pitch_data[1], CL_RGBA, CL_UNORM_INT8);

	{
		cl_uint indices[] = { image_gpu, kernel_gpu, result_image_gpu };
		cl_uint indices_args[] = { width, height, width_filter, height_filter };
		cl_int type_args[] = { sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(convolution_kernel_index, indices, (cl_char*)indices_args, type_args, 3, 4);
	}
	{
		cl_uint indices[] = { result_image_gpu, image_gpu };
		cl_uint indices_args[] = { width, height };
		cl_int type_args[] = { sizeof(cl_uint), sizeof(cl_uint) };
		device->callOpenclFunction(noise_kernel_index, indices, (cl_char*)indices_args, type_args, 2, 2);
	}
	device->readImage((void**)&image, &image_gpu, type_arguments, &width, &height, 1);
	device->freeImageMemory(result_image_gpu);
	device->freeImageMemory(image_gpu);
	device->freeImageMemory(kernel_gpu);
}

MakeNoise::~MakeNoise()
{
	free(kernel);
}