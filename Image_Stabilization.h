#pragma once
#include "clDevice.h"
#include "Load_Save_Images.h"
class Image_Stabilization
{
	cl_int kernel_image_stabilization;
public:
	Image_Stabilization(clDevice* device, std::vector<dataImage> frames, std::vector<dataImage> result_frames);
	~Image_Stabilization();
};

