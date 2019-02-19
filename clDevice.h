
#pragma once

#include "CL/cl.h"
#include "CL/opencl.h"
#include <malloc.h>
#include <stdio.h>
#include <corecrt_memcpy_s.h>
#include <new>
//#pragma comment(lib, "x86_64/OpenCL.lib")
#pragma comment(lib, "OpenCL.lib")
#pragma warning(disable:4996)


struct kernelInformation {
	size_t local_work_size[3];
	size_t prefer_work_group_size;
	size_t max_work_group_size;
	cl_ulong local_size;
	cl_ulong private_memory;
};

struct structDeviceInfo {
	cl_uint maxComputeUnit;
	cl_uint workItemDemension;
	cl_uint maxComputeUnits;
	cl_uint preferVectorChar;
	cl_uint preferVectorShort;
	cl_uint preferVectorInt;
	cl_uint preferVectorLong;
	cl_uint preferVectorFloat;
	cl_uint preferVectorDouble;
	cl_bool supportImages;
	cl_char* deviceVendor;
	cl_char* deviceExtensions;
	size_t workItemSizes[3];
	size_t maximumParametersInKernel;
	size_t maxWorkGroupSize;
	size_t maxHeightImage2D;
	size_t maxWidthImage2D;
	size_t maxHeightImage3D;
	size_t maxWidthImage3D;
	size_t maxDepthImage3D;
	cl_ulong localMemorySize;
	cl_ulong maxGlobalMemoryAllocate;
	cl_ulong globalMemSize;
	cl_device_type deviceType;
	cl_device_local_mem_type localMemoryType;
};


#define CL_CHECK(codeError, stringError)											\
   do {																				\
     if (codeError == CL_SUCCESS)													\
       break;																		\
     printf("\nOpenCL Error: '%s' returned %s!\n", stringError, getInformationError(codeError));	\
     abort();																		\
    } while (0)



class clPlatform
{
	cl_uint numberPlatforms;
	cl_uint numberDevices;
	cl_platform_id* platforms;
	cl_uint* platformDevices;
	cl_device_id* devices;
	cl_command_queue* queue;
	cl_context* context;
	cl_uint getNextPlatform;
public:
	clPlatform();
	~clPlatform();
	cl_platform_id* getPlatformID(cl_uint index);
	cl_device_id* getDeviceID(cl_uint index);
	cl_command_queue* getCommandQueueID(cl_uint index);
	cl_context* getContextID(cl_uint index);
	cl_uint getNumberDevices() { return numberDevices; };
	cl_uint getNumberPlatforms() { return numberPlatforms; };
};
class clDevice
{

	cl_char* profileVersionNameVendorExtensions[5];

	cl_platform_id* platform;
	cl_context* context;
	cl_device_id* device;
	cl_command_queue* queue;
	cl_mem** ptrBufferDevice;
	cl_mem** ptrImageDevice;

	cl_char** namesPrograms;
	cl_char** namesKernels;
	cl_program* programDevice;
	cl_kernel* kernels;
	size_t numberKernels;
	size_t numberPrograms;
	cl_uint numberBufferDevice;
	cl_uint numberImageDevice;
public:
	kernelInformation* kernelInfo;
	structDeviceInfo DeviceInfo;
	bool freeImageMemory(size_t index_image);
	bool freeBufferMemory(size_t address);

	void callOpenclFunction(size_t index_kernel, size_t * indices_buffers, size_t * indices_images, cl_char * indices_arguments, cl_int * size_indices_arguments, size_t number_buffers, size_t number_images, size_t number_arguments, size_t work_size[3]);
	void callOpenclFunction(size_t index_kernel, size_t * indices_buffers, size_t * indices_images, cl_char * indices_arguments, cl_int * size_indices_arguments, size_t number_buffers, size_t number_images, size_t number_arguments, size_t work_size[3], size_t local_work_size[3]);
	cl_bool setArguments(cl_uint index_kernel, size_t * addressMemoryBuffer, cl_uint numberIndicesMemoryBuffer, size_t * addressMemoryImage, cl_uint numberIndicesMemoryImage, cl_char * arguments, cl_int * typeArguments, cl_uint numberArguments, cl_uint index_kernel_arguments);
	cl_bool startCalculate(cl_uint index_kernel, size_t globalWork[3], size_t localWork[3]);

	clDevice(clPlatform* platformData, cl_uint indexDevice);
	bool clPushProgram(cl_char* text, size_t lengthText, const cl_char* options);
	bool clPushKernel(cl_char * text, size_t lengthText); 

	cl_bool copyBufferTo2DImage(size_t image, size_t buffer, size_t width, size_t height);

	cl_bool copy2DImageToBuffer(size_t image, size_t buffer, size_t width, size_t height);

	cl_bool writeBuffer(void * data, size_t buffer, size_t length);

	cl_bool write2DImage(void * data, size_t image, size_t width, size_t height);

	cl_bool readBuffer(void * returnedData, size_t memoryRead, cl_uint lengthWrite);

	cl_bool readImage(void * returnedData, size_t memoryRead, size_t width, size_t height);

	cl_bool readBuffers(void ** returnedData, size_t * memoryRead, cl_uchar * typeArgubentsReturnedData, cl_uint * lengthWrite, cl_uint numberIndicesReadData);

	cl_bool readImages(void ** returnedData, size_t * memoryRead, cl_uchar * typeArgubentsReturnedData, size_t * width, size_t * height, cl_uint numberIndicesReadData);

	size_t mallocBufferMemory(const void* data, size_t lengthData);
	size_t mallocImage2DMemory(const void* data, size_t height, size_t width, size_t rowPitch, size_t typeImage, size_t typeData);
	cl_bool startCalculate(cl_uint index_kernel, size_t globalWork[3]);
	cl_bool copy2DImage(size_t image_src, size_t image_dst, size_t width, size_t height);
	cl_char* getNameKernel(cl_uint index);
	cl_char* getNameProgram(cl_uint index);
	cl_int findKernel(const cl_char* text, size_t length);
	~clDevice();
};
