#pragma once
#include "glDevice.h"
const char* set_area_view =
	#include "area_view.glsl"
;
class glImageView
{
public:
	glImageView(size_t ptr_textures, size_t number_textures);
	~glImageView();
};

