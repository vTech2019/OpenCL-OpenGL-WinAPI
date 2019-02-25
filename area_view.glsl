

R"===(

#version 430 compatibility
#extension GL_ARB_compute_shader : enable

uniform vec2 number_block_x_y;
uniform vec3 start;
uniform vec3 vector_build;
uniform vec3 format_block;

layout(std430) buffer Positions
{
	vec3 position[];
};

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main(void)
{
	const int idx = int(gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationID.x);
	const int idy = int(gl_WorkGroupID.y * gl_WorkGroupSize.y + gl_LocalInvocationID.y);
	const int stride_y = gl_NumWorkGroups.y * gl_WorkGroupSize.y;
	const int stride_x = gl_NumWorkGroups.x * gl_WorkGroupSize.x;
	const int number_x = number_block_x_y.x;
	const int number_y = number_block_x_y.y;
	for (int i = idy; i < number_y; i += stride_y) {
		for (int j = idx; j < number_x; j += stride_x) {
			if (i % 2 == 0) {
				vec3 _start = start ;

			}
			else {
			}
			position[(i * number_x + j) * 2] = 

	}
}
)==="