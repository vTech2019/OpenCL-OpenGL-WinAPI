#include "clDevice.h"

void CL_CALLBACK pfnBuildProgram(cl_program program, void *userData)
{
	cl_device_id *deviceID = (cl_device_id*)userData;
	cl_build_status status;
	size_t length = 0;
	cl_int ret = clGetProgramBuildInfo(program, *deviceID, CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &status, &length);
	if (status == CL_BUILD_IN_PROGRESS)
	{
		cl_int ret = clGetProgramBuildInfo(program, *deviceID, CL_PROGRAM_BUILD_LOG, NULL, NULL, &length);
		if (ret != CL_SUCCESS)
			printf("clGetProgramBuildInfo - %d\n", ret);
		cl_char* data = (cl_char*)malloc(length + 1);
		data[length] = 0;
		ret = clGetProgramBuildInfo(program, *deviceID, CL_PROGRAM_BUILD_LOG, length, data, NULL);
		if (ret != CL_SUCCESS)
			printf("clGetProgramBuildInfo - %d\n", ret);
		printf("CL_BUILD_IN_PROGRESS - %s\n", data);
		free(data);
	}
	else if (status == CL_BUILD_ERROR)
	{
		cl_int ret = clGetProgramBuildInfo(program, *deviceID, CL_PROGRAM_BUILD_LOG, NULL, NULL, &length);
		if (ret != CL_SUCCESS)
			printf("clGetProgramBuildInfo - %d\n", ret);
		cl_char* data = (cl_char*)malloc(length + 1);
		data[length] = 0;
		ret = clGetProgramBuildInfo(program, *deviceID, CL_PROGRAM_BUILD_LOG, length, data, NULL);
		if (ret != CL_SUCCESS)
			printf("clGetProgramBuildInfo - %d\n", ret);
		printf("CL_BUILD_ERROR - %s\n", data);
		free(data);
	}
	else if (status == CL_BUILD_NONE) {
		printf("CL_BUILD_NONE\n");
	}
	else if (status == CL_BUILD_SUCCESS) {
		printf("CL_BUILD_SUCCESS\n");
	}
}

const char* getInformationError(cl_int codeError)
{
	switch (codeError)
	{
	case CL_SUCCESS:
		return "CL_SUCCESS";
	case CL_DEVICE_NOT_FOUND:
		return "CL_DEVICE_NOT_FOUND";
	case CL_DEVICE_NOT_AVAILABLE:
		return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:
		return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:
		return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:
		return "CL_OUT_OF_HOST_MEMORY";
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:
		return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:
		return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:
		return "CL_BUILD_PROGRAM_FAILURE";
	case CL_MAP_FAILURE:
		return "CL_MAP_FAILURE";
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:
		return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:
		return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
	case CL_COMPILE_PROGRAM_FAILURE:
		return "CL_COMPILE_PROGRAM_FAILURE";
	case CL_LINKER_NOT_AVAILABLE:
		return "CL_LINKER_NOT_AVAILABLE";
	case CL_LINK_PROGRAM_FAILURE:
		return "CL_LINK_PROGRAM_FAILURE";
	case CL_DEVICE_PARTITION_FAILED:
		return "CL_DEVICE_PARTITION_FAILED";
	case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:
		return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";
	case CL_INVALID_VALUE:
		return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:
		return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:
		return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:
		return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:
		return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:
		return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:
		return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:
		return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:
		return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:
		return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:
		return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:
		return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:
		return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:
		return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:
		return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:
		return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:
		return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:
		return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:
		return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:
		return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:
		return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:
		return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:
		return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:
		return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:
		return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:
		return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:
		return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:
		return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:
		return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:
		return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:
		return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:
		return "CL_INVALID_MIP_LEVEL";
	case CL_INVALID_GLOBAL_WORK_SIZE:
		return "CL_INVALID_GLOBAL_WORK_SIZE";
	case CL_INVALID_PROPERTY:
		return "CL_INVALID_PROPERTY";
	case CL_INVALID_IMAGE_DESCRIPTOR:
		return "CL_INVALID_IMAGE_DESCRIPTOR";
	case CL_INVALID_COMPILER_OPTIONS:
		return "CL_INVALID_COMPILER_OPTIONS";
	case CL_INVALID_LINKER_OPTIONS:
		return "CL_INVALID_LINKER_OPTIONS";
	case CL_INVALID_DEVICE_PARTITION_COUNT:
		return "CL_INVALID_DEVICE_PARTITION_COUNT";
	case -9999:
		return "Illegal read or write to a buffer (NVIDIA)";
	default:
		return "CL_INVALID_INDEX";
	}
}

void platformInfo(cl_platform_id platform, cl_platform_info information, const cl_char textInformation[], cl_char* strPlatformInfo[5]) {
	size_t length;
	size_t typeInfo = information - 0x900;
	CL_CHECK(clGetPlatformInfo(platform, information, NULL, NULL, &length), textInformation);
	strPlatformInfo[typeInfo] = (cl_char*)malloc(length + 1);
	strPlatformInfo[typeInfo][length] = 0;
	CL_CHECK(clGetPlatformInfo(platform, information, length, strPlatformInfo[typeInfo], NULL), textInformation);
	printf("%s - %s\n", textInformation, strPlatformInfo[typeInfo]);
}

void deviceInfo(cl_device_id device, cl_platform_info information, const cl_char textInformation[], void** data, cl_uint lengthData) {
	size_t length;
	CL_CHECK(clGetDeviceInfo(device, information, NULL, NULL, &length), textInformation);
	if (lengthData != length) {
		*data = (cl_char*)calloc((length + 1), sizeof(cl_char));
		CL_CHECK(clGetDeviceInfo(device, information, length, *data, NULL), textInformation);
		printf("%s - %s\n", textInformation, *data);
	}
	else {
		CL_CHECK(clGetDeviceInfo(device, information, length, &(*((cl_char*)data)), NULL), textInformation);
	}
}

clPlatform::clPlatform() {
	for (size_t i = 0; i < sizeof(*this); i++)
		((cl_char*)this)[i] = 0;
	cl_int errorCode;
	CL_CHECK(clGetPlatformIDs(NULL, NULL, &numberPlatforms), "clGetPlatformIDs");
	platforms = (cl_platform_id*)malloc(numberPlatforms * sizeof(cl_platform_id));
	context = (cl_context*)malloc(numberPlatforms * sizeof(cl_context));
	platformDevices = (cl_uint*)malloc(numberPlatforms * sizeof(cl_uint));
	CL_CHECK(clGetPlatformIDs(numberPlatforms, platforms, NULL), "clGetPlatformIDs");

	if (numberPlatforms == 0) return;

	CL_CHECK(clGetDeviceIDs(platforms[getNextPlatform], CL_DEVICE_TYPE_ALL, NULL, NULL, &platformDevices[getNextPlatform]), "clGetDeviceIDs");
	devices = (cl_device_id*)realloc(devices, (platformDevices[getNextPlatform] + numberDevices) * sizeof(cl_device_id));
	queue = (cl_command_queue*)realloc(queue, (platformDevices[getNextPlatform] + numberDevices) * sizeof(cl_command_queue));
	CL_CHECK(clGetDeviceIDs(platforms[getNextPlatform], CL_DEVICE_TYPE_ALL, platformDevices[getNextPlatform], devices + numberDevices, NULL), "clGetDeviceIDs");
	cl_context_properties properties[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[getNextPlatform], 0 };
	context[getNextPlatform] = clCreateContext(properties, platformDevices[getNextPlatform], devices + numberDevices, NULL, NULL, &errorCode);
	CL_CHECK(errorCode, "clCreateContext");
	for (cl_uint j = 0; j < platformDevices[getNextPlatform]; j++) {
		queue[numberDevices + j] = clCreateCommandQueue(context[getNextPlatform], devices[numberDevices + j], CL_QUEUE_PROFILING_ENABLE, &errorCode);
		CL_CHECK(errorCode, "clCreateCommandQueue");
	}
	numberDevices += platformDevices[getNextPlatform];
	getNextPlatform++;
}

cl_command_queue* clPlatform::getCommandQueueID(cl_uint index) {
	return index < numberDevices ? &queue[index] : NULL;
}
cl_device_id* clPlatform::getDeviceID(cl_uint index) {
	return index < numberDevices ? &devices[index] : NULL;
}
cl_platform_id* clPlatform::getPlatformID(cl_uint index) {
	if (index > numberDevices)
		return NULL;
	cl_uint sum = 0;
	for (size_t i = 0; i < numberPlatforms; i++) {
		sum += platformDevices[i];
		if (index < sum)
			return &platforms[i];
	}
	return NULL;
}
cl_context* clPlatform::getContextID(cl_uint index) {
	if (index > numberDevices)
		return NULL;
	cl_uint sum = 0;
	for (cl_uint i = 0; i < numberPlatforms; i++) {
		sum += platformDevices[i];
		if (index < sum)
			return &context[i];
	}
	return NULL;
}

clPlatform::~clPlatform() {
	for (cl_uint i = 0; i < numberDevices; i++) {
		clReleaseCommandQueue(queue[i]);
		clReleaseDevice(devices[i]);
	}
	for (cl_uint i = 0; i < numberPlatforms; i++)
		clReleaseContext(context[i]);
	if (context) free(context);
	if (queue) free(queue);
	if (devices) free(devices);
	if (platforms) free(platforms);
	if (platformDevices) free(platformDevices);
}
clDevice::clDevice(clPlatform* platformData, cl_uint indexDevice)
{
	memset(this, 0, sizeof(*this));
	if (platformData->getNumberDevices() < indexDevice) return;
	platform = platformData->getPlatformID(indexDevice);
	context = platformData->getContextID(indexDevice);
	device = platformData->getDeviceID(indexDevice);
	queue = platformData->getCommandQueueID(indexDevice);

	platformInfo(*platform, CL_PLATFORM_VENDOR, (const cl_char*)"CL_PLATFORM_VENDOR", profileVersionNameVendorExtensions);
	platformInfo(*platform, CL_PLATFORM_NAME, (const cl_char*)"CL_PLATFORM_NAME", profileVersionNameVendorExtensions);
	platformInfo(*platform, CL_PLATFORM_VERSION, (const cl_char*)"CL_PLATFORM_VERSION", profileVersionNameVendorExtensions);
	platformInfo(*platform, CL_PLATFORM_PROFILE, (const cl_char*)"CL_PLATFORM_PROFILE", profileVersionNameVendorExtensions);
	platformInfo(*platform, CL_PLATFORM_EXTENSIONS, (const cl_char*)"CL_PLATFORM_EXTENSIONS", profileVersionNameVendorExtensions);

	deviceInfo(*device, CL_DEVICE_EXTENSIONS, (const cl_char*)"CL_DEVICE_EXTENSIONS", (void**)&DeviceInfo.deviceExtensions, 0);
	deviceInfo(*device, CL_DEVICE_VENDOR, (const cl_char*)"CL_DEVICE_VENDOR_ID", (void**)&DeviceInfo.deviceVendor, 0);
	deviceInfo(*device, CL_DEVICE_TYPE, (const cl_char*)"CL_DEVICE_TYPE", (void**)&DeviceInfo.deviceType, sizeof(cl_device_type));
	deviceInfo(*device, CL_DEVICE_LOCAL_MEM_TYPE, (const cl_char*)"CL_DEVICE_LOCAL_MEM_TYPE", (void**)&DeviceInfo.localMemoryType, sizeof(cl_device_local_mem_type));
	deviceInfo(*device, CL_DEVICE_MAX_COMPUTE_UNITS, (const cl_char*)"CL_DEVICE_MAX_COMPUTE_UNITS", (void**)&DeviceInfo.maxComputeUnits, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, (const cl_char*)"CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS", (void**)&DeviceInfo.workItemDemension, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_MAX_WORK_ITEM_SIZES, (const cl_char*)"CL_DEVICE_MAX_WORK_ITEM_SIZES", (void**)&DeviceInfo.workItemSizes, 3 * sizeof(size_t));
	deviceInfo(*device, CL_DEVICE_LOCAL_MEM_SIZE, (const cl_char*)"CL_DEVICE_LOCAL_MEM_SIZE", (void**)&DeviceInfo.localMemorySize, sizeof(cl_ulong));
	deviceInfo(*device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, (const cl_char*)"CL_DEVICE_MAX_MEM_ALLOC_SIZE", (void**)&DeviceInfo.maxGlobalMemoryAllocate, sizeof(cl_ulong));
	deviceInfo(*device, CL_DEVICE_MAX_PARAMETER_SIZE, (const cl_char*)"CL_DEVICE_MAX_PARAMETER_SIZE", (void**)&DeviceInfo.maximumParametersInKernel, sizeof(size_t));
	deviceInfo(*device, CL_DEVICE_MAX_WORK_GROUP_SIZE, (const cl_char*)"CL_DEVICE_MAX_WORK_GROUP_SIZE", (void**)&DeviceInfo.maxWorkGroupSize, sizeof(size_t));
	deviceInfo(*device, CL_DEVICE_GLOBAL_MEM_SIZE, (const cl_char*)"CL_DEVICE_GLOBAL_MEM_SIZE", (void**)&DeviceInfo.globalMemSize, sizeof(cl_ulong));
	deviceInfo(*device, CL_DEVICE_IMAGE_SUPPORT, (const cl_char*)"CL_DEVICE_IMAGE_SUPPORT", (void**)&DeviceInfo.supportImages, sizeof(cl_bool));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR", (void**)&DeviceInfo.preferVectorChar, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT", (void**)&DeviceInfo.preferVectorShort, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT", (void**)&DeviceInfo.preferVectorInt, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG", (void**)&DeviceInfo.preferVectorLong, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT", (void**)&DeviceInfo.preferVectorFloat, sizeof(cl_uint));
	deviceInfo(*device, CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, (const cl_char*)"CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE", (void**)&DeviceInfo.preferVectorDouble, sizeof(cl_uint));
	if (DeviceInfo.supportImages) {
		deviceInfo(*device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, (const cl_char*)"CL_DEVICE_IMAGE2D_MAX_HEIGHT", (void**)&DeviceInfo.maxHeightImage2D, sizeof(size_t));
		deviceInfo(*device, CL_DEVICE_IMAGE2D_MAX_WIDTH, (const cl_char*)"CL_DEVICE_IMAGE2D_MAX_WIDTH", (void**)&DeviceInfo.maxWidthImage2D, sizeof(size_t));
		deviceInfo(*device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, (const cl_char*)"CL_DEVICE_IMAGE3D_MAX_HEIGHT", (void**)&DeviceInfo.maxHeightImage3D, sizeof(size_t));
		deviceInfo(*device, CL_DEVICE_IMAGE3D_MAX_WIDTH, (const cl_char*)"CL_DEVICE_IMAGE3D_MAX_WIDTH", (void**)&DeviceInfo.maxWidthImage3D, sizeof(size_t));
		deviceInfo(*device, CL_DEVICE_IMAGE3D_MAX_DEPTH, (const cl_char*)"CL_DEVICE_IMAGE3D_MAX_DEPTH", (void**)&DeviceInfo.maxDepthImage3D, sizeof(size_t));
	}
}

bool clDevice::clPushProgram(cl_char * text, size_t lengthText, const cl_char* options)
{
	cl_uint number_kernels;
	cl_int errorCode;
	namesPrograms = (cl_char**)realloc(namesPrograms, (numberPrograms + 1) * sizeof(cl_char*));
	namesPrograms[numberPrograms] = (cl_char*)malloc(lengthText * sizeof(cl_char));
	memcpy(namesPrograms[numberPrograms], text, lengthText);
	printf("-----------------Program---------------------\n");
	//printf("%s \n", namesPrograms[numberPrograms]);
	printf("---------------------------------------------\n");
	programDevice = (cl_program*)realloc(programDevice, (numberPrograms + 1) * sizeof(cl_program));
	programDevice[numberPrograms] = clCreateProgramWithSource(*context, 1, (const char**)&text, NULL, &errorCode);
	CL_CHECK(errorCode, "clCreateProgramWithSource");
	CL_CHECK(clBuildProgram(programDevice[numberPrograms], 1, device, (const char*)options, pfnBuildProgram, (void*)device), "clBuildProgram");
	CL_CHECK(clCreateKernelsInProgram(programDevice[numberPrograms], NULL, NULL, &number_kernels), "clCreateKernelsInProgram");
	if (number_kernels) {
		kernels = (cl_kernel*)realloc(kernels, (numberKernels + number_kernels) * sizeof(cl_kernel));
		CL_CHECK(clCreateKernelsInProgram(programDevice[numberPrograms], number_kernels, kernels + numberKernels, NULL), "clCreateKernelsInProgram");
		namesKernels = (cl_char**)realloc(namesKernels, (numberKernels + number_kernels) * sizeof(cl_char*));
		kernelInfo = (kernelInformation*)realloc(kernelInfo, (numberKernels + number_kernels) * sizeof(kernelInformation));
		printf("-----------------kernels---------------------\n");
		for (size_t i = numberKernels; i < numberKernels + number_kernels; i++) {
			cl_char* kernel_name = NULL;
			size_t length_name_kernel;
			CL_CHECK(clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, NULL, NULL, &length_name_kernel), "clGetKernelInfo");
			namesKernels[i] = (cl_char*)malloc((length_name_kernel + 1) * sizeof(cl_char));
			CL_CHECK(clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, length_name_kernel, namesKernels[i], &length_name_kernel), "clGetKernelInfo");
			namesKernels[i][length_name_kernel] = 0;
			CL_CHECK(clGetKernelWorkGroupInfo(kernels[i], *device, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizeof(size_t[3]), &kernelInfo[i].local_work_size, NULL), "clGetKernelWorkGroupInfo");
			CL_CHECK(clGetKernelWorkGroupInfo(kernels[i], *device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelInfo[i].max_work_group_size, NULL), "clGetKernelWorkGroupInfo");
			CL_CHECK(clGetKernelWorkGroupInfo(kernels[i], *device, CL_KERNEL_LOCAL_MEM_SIZE, sizeof(cl_ulong), &kernelInfo[i].local_size, NULL), "clGetKernelWorkGroupInfo");
			CL_CHECK(clGetKernelWorkGroupInfo(kernels[i], *device, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &kernelInfo[i].prefer_work_group_size, NULL), "clGetKernelWorkGroupInfo");
			CL_CHECK(clGetKernelWorkGroupInfo(kernels[i], *device, CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(cl_ulong), &kernelInfo[i].private_memory, NULL), "clGetKernelWorkGroupInfo");
			printf("index kernel : %zi; name kernel : %s \n", i, namesKernels[i]);
		}
		printf("---------------------------------------------\n");
		numberKernels += number_kernels;
	}
	numberPrograms++;
	return true;
}

cl_int clDevice::findKernel(const cl_char* text, size_t length) {
	bool find = false;
	for (size_t i = 0; i < numberKernels; i++) {
		for (size_t j = 0; j < length && namesKernels[i][j] != 0 && namesKernels[i][j] == text[j]; j++) {
			if (text[j + 1] == 0 && namesKernels[i][j + 1] == 0)
				find = true;
		}
		if (find)
			return i;
	}
	return -1;
}

bool clDevice::clPushKernel(cl_char * text, size_t lengthText)
{
	cl_int errorCode;
	cl_kernel kernel = NULL;
	for (size_t i = 0; i < numberPrograms; i++) {
		kernel = clCreateKernel(programDevice[i], (const char*)text, &errorCode);
		if (CL_INVALID_KERNEL_NAME != errorCode)
			CL_CHECK(errorCode, "clCreateKernel");
	}
	if (kernel == NULL)
		return false;
	namesKernels = (cl_char**)realloc(namesKernels, (numberKernels + 1) * sizeof(cl_char*));
	namesKernels[numberKernels] = (cl_char*)malloc((1 + lengthText) * sizeof(cl_char));
	memcpy(namesKernels[numberKernels], text, lengthText);
	namesKernels[numberKernels][lengthText] = 0;
	kernels = (cl_kernel*)realloc(kernels, (numberKernels + 1) * sizeof(cl_kernel));
	kernels[numberKernels] = kernel;
	kernelInfo = (kernelInformation*)realloc(kernelInfo, (numberKernels + 1) * sizeof(kernelInformation));
	clGetKernelWorkGroupInfo(kernel, *device, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, sizeof(size_t[3]), &kernelInfo[numberKernels].local_work_size, NULL);
	clGetKernelWorkGroupInfo(kernel, *device, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &kernelInfo[numberKernels].max_work_group_size, NULL);
	clGetKernelWorkGroupInfo(kernel, *device, CL_KERNEL_LOCAL_MEM_SIZE, sizeof(cl_ulong), &kernelInfo[numberKernels].local_size, NULL);
	clGetKernelWorkGroupInfo(kernel, *device, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, sizeof(size_t), &kernelInfo[numberKernels].prefer_work_group_size, NULL);
	clGetKernelWorkGroupInfo(kernel, *device, CL_KERNEL_PRIVATE_MEM_SIZE, sizeof(cl_ulong), &kernelInfo[numberKernels].private_memory, NULL);
	numberKernels++;
	return true;
}

cl_uint clDevice::mallocBufferMemory(const void* data, size_t lengthData) {
	ptrMemoryDevice = (cl_mem*)realloc(ptrMemoryDevice, (numberMemoryDevice + 1) * sizeof(cl_mem));
	cl_int errors;
	if ((void*)data)
		ptrMemoryDevice[numberMemoryDevice] = clCreateBuffer(*context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, lengthData, (void*)data, &errors);
	else
		ptrMemoryDevice[numberMemoryDevice] = clCreateBuffer(*context, CL_MEM_READ_WRITE, lengthData, NULL, &errors);
	CL_CHECK(errors, "clCreateBuffer");
	if (errors) {
		ptrMemoryDevice = (cl_mem*)realloc(ptrMemoryDevice, (numberMemoryDevice) * sizeof(cl_mem));
		return NULL;
	}
	numberMemoryDevice += 1;
	return numberMemoryDevice - 1;
}

cl_uint clDevice::mallocImageMemory(const void* data, size_t height, size_t width, size_t rowPitch, size_t typeImage, size_t typeData) {
	ptrImageDevice = (cl_mem*)realloc(ptrImageDevice, (numberImageDevice + 1) * sizeof(cl_mem));
	cl_int errors;
	cl_image_format clImageFormat;
	if ((void*)data) {
		clImageFormat.image_channel_order = typeImage;
		clImageFormat.image_channel_data_type = typeData;
		ptrImageDevice[numberImageDevice] = clCreateImage2D(*context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, &clImageFormat, width, height, rowPitch, (void*)data, &errors);
	}
	else {
		clImageFormat.image_channel_order = typeImage;
		clImageFormat.image_channel_data_type = typeData;
		ptrImageDevice[numberImageDevice] = clCreateImage2D(*context, CL_MEM_READ_WRITE, &clImageFormat, width, height, NULL, NULL, &errors);
	}
	CL_CHECK(errors, "clCreateImage2D");
	if (errors) {
		ptrImageDevice = (cl_mem*)realloc(ptrImageDevice, (numberImageDevice) * sizeof(cl_mem));
		return SIZE_MAX;
	}
	numberImageDevice += 1;
	return numberImageDevice - 1;
}

bool clDevice::freeImageMemory(size_t indexMemory) {
	if (indexMemory < numberImageDevice) {
		CL_CHECK(clReleaseMemObject(ptrImageDevice[indexMemory]), "clReleaseMemObject");

		if (indexMemory == numberImageDevice - 1) {
			numberImageDevice--;
			ptrImageDevice = (cl_mem*)realloc(ptrImageDevice, (numberImageDevice) * sizeof(cl_mem));
		}
		else {
			numberImageDevice--;
			cl_mem* tmp_objects = (cl_mem*)malloc((numberImageDevice - indexMemory) * sizeof(cl_mem));
			memcpy(tmp_objects, ptrImageDevice + indexMemory + 1, (numberImageDevice - indexMemory) * sizeof(cl_mem));
			memcpy(ptrImageDevice + indexMemory, tmp_objects, (numberImageDevice - indexMemory) * sizeof(cl_mem));
			ptrImageDevice = (cl_mem*)realloc(ptrImageDevice, (numberImageDevice) * sizeof(cl_mem));
			free(tmp_objects);
		}
		return true;
	}
	return false;
}

void clDevice::popBufferMemory() {
	if (numberMemoryDevice > 0) {
		numberMemoryDevice--;
		CL_CHECK(clReleaseMemObject(ptrMemoryDevice[numberMemoryDevice]), "clReleaseMemObject");
		ptrMemoryDevice = (cl_mem*)realloc(ptrMemoryDevice, (numberMemoryDevice) * sizeof(cl_mem));
	}
}
void clDevice::popImageMemory() {
	if (numberImageDevice > 0) {
		numberImageDevice--;
		CL_CHECK(clReleaseMemObject(ptrImageDevice[numberImageDevice]), "clReleaseMemObject");
		ptrImageDevice = (cl_mem*)realloc(ptrImageDevice, (numberImageDevice) * sizeof(cl_mem));
	}
}
void clDevice::callOpenclFunction(size_t index_kernel, cl_uint* indices_images, cl_char* indices_arguments, cl_int* size_indices_arguments, size_t number_images, size_t number_arguments) {
	cl_uint* index_kernel_buffer = (cl_uint*)_alloca(number_images * sizeof(cl_uint));
	cl_uint* index_kernel_arguments = (cl_uint*)_alloca(number_arguments * sizeof(cl_uint));
	size_t i = 0;
	for (; i < number_images; i++)
		index_kernel_buffer[i] = i;
	for (size_t j = 0; i < number_images + number_arguments; i++) {
		index_kernel_arguments[j++] = i;
	}
	size_t work_size[3];
	cl_char* ptr_indices_arguments = indices_arguments;
	for (size_t i = 0; i < 2; i++) {
		switch (size_indices_arguments[i]) {
		case 1:
			work_size[i] = *((cl_uchar*)ptr_indices_arguments);
			ptr_indices_arguments += 1;
			break;
		case 2:
			work_size[i] = *((cl_short*)ptr_indices_arguments);
			ptr_indices_arguments += 2;
			break;
		case 4:
			work_size[i] = *((cl_uint*)ptr_indices_arguments);
			ptr_indices_arguments += 4;
			break;
		case 8:
			work_size[i] = *((cl_ulong*)ptr_indices_arguments);
			ptr_indices_arguments += 8;
			break;
		}
	}
	work_size[2] = 1;
	this->setArguments(index_kernel, NULL, NULL, indices_images, number_images, index_kernel_buffer, indices_arguments, size_indices_arguments, number_arguments, index_kernel_arguments);
	this->startCalculate(index_kernel, work_size);
}
void clDevice::callOpenclFunction(size_t index_kernel, cl_uint* indices_images, cl_char* indices_arguments, cl_int* size_indices_arguments, size_t number_images, size_t number_arguments, size_t x_work, size_t y_work, size_t z_work) {
	cl_uint* index_kernel_buffer = (cl_uint*)_alloca(number_images * sizeof(cl_uint));
	cl_uint* index_kernel_arguments = (cl_uint*)_alloca(number_arguments * sizeof(cl_uint));
	size_t i = 0;
	for (; i < number_images; i++)
		index_kernel_buffer[i] = i;
	for (size_t j = 0; i < number_images + number_arguments; i++) {
		index_kernel_arguments[j++] = i;
	}
	size_t work_size[3] = { x_work, y_work, z_work };
	this->setArguments(index_kernel, NULL, NULL, indices_images, number_images, index_kernel_buffer, indices_arguments, size_indices_arguments, number_arguments, index_kernel_arguments);
	this->startCalculate(index_kernel, work_size);
}

cl_bool clDevice::setArguments(cl_uint index_kernel, cl_uint* indicesMemoryBuffer, cl_uint numberIndicesMemoryBuffer, cl_uint* indicesMemoryImage, cl_uint numberIndicesMemoryImage, cl_uint* index_kernel_buffer,
	void* arguments, cl_int* typeArguments, cl_uint numberArguments, cl_uint* index_kernel_arguments) {
	for (size_t i = 0; i < numberIndicesMemoryBuffer; i++) {
		if (indicesMemoryBuffer[i] < numberMemoryDevice)
			CL_CHECK(clSetKernelArg(kernels[index_kernel], index_kernel_buffer[i], sizeof(ptrMemoryDevice[indicesMemoryBuffer[i]]), &ptrMemoryDevice[indicesMemoryBuffer[i]]), "clSetKernelArg");
		else {
			printf("Error index in clSetKernelArg");
			return false;
		}
	}
	for (size_t i = 0; i < numberIndicesMemoryImage; i++) {
		if (indicesMemoryImage[i] < numberImageDevice)
			CL_CHECK(clSetKernelArg(kernels[index_kernel], index_kernel_buffer[i], sizeof(ptrImageDevice[indicesMemoryImage[i]]), &ptrImageDevice[indicesMemoryImage[i]]), "clSetKernelArg");
		else {
			printf("Error index in clSetKernelArg");
			return false;
		}
	}
	size_t offset = 0;
	cl_char* _arguments = (cl_char*)arguments;
	for (size_t i = 0; i < numberArguments; i++) {
		if (typeArguments[i] < 0) {
			const int length_local_memory = -typeArguments[i];
			CL_CHECK(clSetKernelArg(kernels[index_kernel], index_kernel_arguments[i], length_local_memory, NULL), "clSetKernelArg");
			offset += sizeof(NULL);
		}
		else {
			CL_CHECK(clSetKernelArg(kernels[index_kernel], index_kernel_arguments[i], typeArguments[i], &_arguments[offset]), "clSetKernelArg");
			offset += typeArguments[i];
		}
	}
	return true;
}

cl_bool clDevice::startCalculate(cl_uint index_kernel, size_t globalWork[3]) {
	cl_event kernelEvent;
	size_t x = sqrt(kernelInfo[index_kernel].max_work_group_size);
	size_t y = kernelInfo[index_kernel].max_work_group_size / x;
	size_t localWork[3] = { x, y, 1 };
	if (globalWork[0] % localWork[0])
		globalWork[0] += localWork[0] - globalWork[0] % localWork[0];
	if (globalWork[1] % localWork[1])
		globalWork[1] += localWork[1] - globalWork[1] % localWork[1];
	if (globalWork[2] % localWork[2])
		globalWork[2] += localWork[2] - globalWork[2] % localWork[2];
	CL_CHECK(clEnqueueNDRangeKernel(*queue, kernels[index_kernel], 2, NULL, globalWork, localWork, NULL, NULL, &kernelEvent), "clEnqueueNDRangeKernel");

	cl_ulong time_start, time_end;
	CL_CHECK(clWaitForEvents(1, &kernelEvent), "clWaitForEvents");
	CL_CHECK(clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL), "clGetEventProfilingInfo");
	CL_CHECK(clGetEventProfilingInfo(kernelEvent, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL), "clGetEventProfilingInfo");
	const cl_ulong total_time = time_end - time_start;
	const cl_double time = total_time / 1000000.0;
	printf("Execution time:\t\t%0f ms\n", time);
	return true;
}
cl_bool clDevice::copy2DImage(cl_uint image_src, cl_uint image_dst, size_t width, size_t height) {
	if (image_src < numberImageDevice && image_dst < numberImageDevice) {
		size_t region[] = { width, height , 1 };
		size_t  src_origin[] = { 0, 0, 0 };
		size_t dst_origin[] = { 0, 0, 0 };
		CL_CHECK(clEnqueueCopyImage(*queue, ptrImageDevice[image_src], ptrImageDevice[image_dst], src_origin, dst_origin, region, NULL, NULL, NULL), "clEnqueueCopyImage");
	}
	return true;
}
cl_bool clDevice::readBuffer(void** returnedData, cl_uint* indicesReadData, cl_uchar* typeArgubentsReturnedData, cl_ulong* lengthWrite, cl_uint numberIndicesReadData) {
	cl_char** hostData = (cl_char**)returnedData;
	size_t offset = 0;
	for (size_t i = 0; i < numberIndicesReadData; i++) {
		if (indicesReadData[i] < numberMemoryDevice)
			CL_CHECK(clEnqueueReadBuffer(*queue, ptrMemoryDevice[indicesReadData[i]], CL_TRUE, 0, lengthWrite[i], hostData[offset], NULL, NULL, NULL), "clEnqueueReadBuffer");
		offset += typeArgubentsReturnedData[i];
	}
	return true;
}
cl_bool clDevice::readImage(void** returnedData, cl_uint* indicesReadData, cl_uchar* typeArgubentsReturnedData, size_t* width, size_t* height, cl_uint numberIndicesReadData) {
	cl_char** hostData = (cl_char**)returnedData;
	size_t offset = 0;

	for (size_t i = 0; i < numberIndicesReadData; i++) {
		if (indicesReadData[i] < numberImageDevice) {
			const size_t offsetImages[3] = { 0, 0, 0 };
			const size_t sizeRegion[3] = { width[i], height[i], 1 };
			CL_CHECK(clEnqueueReadImage(*queue, ptrImageDevice[indicesReadData[i]], CL_TRUE, offsetImages, sizeRegion, NULL, NULL, hostData[offset], NULL, NULL, NULL), "clEnqueueReadImage");
		}
		if (typeArgubentsReturnedData)
			offset += typeArgubentsReturnedData[i];
	}
	return true;
}
cl_char* clDevice::getNameKernel(cl_uint index) {
	return numberKernels > index ? namesKernels[index] : NULL;
}
cl_char* clDevice::getNameProgram(cl_uint index) {
	return numberPrograms > index ? namesPrograms[index] : NULL;
}
clDevice::~clDevice()
{
	free(profileVersionNameVendorExtensions[0]);
	free(profileVersionNameVendorExtensions[1]);
	free(profileVersionNameVendorExtensions[2]);
	free(profileVersionNameVendorExtensions[3]);
	free(profileVersionNameVendorExtensions[4]);

	for (size_t i = 0; i < numberKernels; i++)
		clReleaseKernel(kernels[i]),
		free(namesKernels[i]);
	if (kernelInfo)
		free(kernelInfo);
	for (size_t i = 0; i < numberPrograms; i++)
		clReleaseProgram(programDevice[i]),
		free(namesPrograms[i]);
	if (*context)
		clReleaseContext(*context);
	if (kernels) free(kernels);
	if (programDevice) free(programDevice);
	if (namesPrograms) free(namesPrograms);
	if (namesKernels) free(namesKernels);
}