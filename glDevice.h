#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "Structures.h"
#include <immintrin.h>
#define GLEW_STATIC
#include "glew/include/GL/glew.h"
#ifndef _WIN64
#pragma comment(lib, "glew/lib/Release/Win32/glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#else
#pragma comment(lib, "glew/lib/Release/x64/glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#endif

struct gl_texture_list {
	unsigned int type_memory;
	unsigned int memory_data;
	gl_texture_list* previous;
	gl_texture_list* next;
};
struct gl_program_list {
	unsigned int memory_data;
	gl_program_list* previous;
	gl_program_list* next;
};
struct gl_buffer_list {
	GLuint ptr_buffer;
	GLuint type_buffer;
	gl_buffer_list* previous;
	gl_buffer_list* next;
};
struct gl_VAO_list {
	GLuint VAO;
	GLuint numberElements;
	GLuint typeFigure;
	gl_VAO_list* previous;
	gl_VAO_list* next;
};
	void Draw(void * argument);
class glDevice
{
	matrix_16  projectionMatrix;
	matrix_16 matrixView;
	matrix_16  matrixRotate;
	my_vec3 Eye;
	my_vec3 Center;
	my_vec3 Up;
	gl_texture_list* ptrTexturesDevice;
	gl_texture_list* headPtrTexturesDevice;
	gl_program_list* ptrProgramsDevice;
	gl_program_list* headPtrProgramsDevice;
	gl_buffer_list* ptrBufferDevice;
	gl_buffer_list* headPtrBufferDevice;
	gl_VAO_list* ptrVAODevice;
	gl_VAO_list* headPtrVAODevice;

	GLuint* fragmentShaders;
	GLuint* vertexShaders;
	GLuint* computeShaders;
	size_t numberFragmentShaders;
	size_t numberVertexShaders;
	size_t numberComputeShaders;
	size_t numberObjectsVAO;
	void checkErrorShader(GLuint shader, const GLchar * text, GLuint status);
	void programInfoLog(GLuint shader); 
public:
	size_t push2DTexture(GLubyte4* image, GLuint width, GLuint height);
	size_t push2DTexture(GLubyte* image, GLuint width, GLuint height);
	bool pushShaders(GLuint * typeShader, GLchar ** code, size_t length);

	size_t pushProgram();

	size_t pushVAO(void * data, size_t lengthData, size_t numberCoordinates, GLuint figure);

	void pushBufferInVAO(size_t gl_buffer, size_t gl_VAO, size_t numberCoordinates, GLint attributeLocation);


	size_t pushBuffer(void * data, size_t lengthData, size_t typeBuffer, GLenum renderingMode);

	void useProgram(size_t address);

	void useTexture(size_t address);

	void useBuffer(size_t address);

	void useVAO(size_t address);

	void glDevice::setViewport(GLint x, GLint y, GLint width, GLint height);

	GLfloat * returnMatrixView();

	GLfloat * returnMatrixProjection();

	GLfloat * returnMatrixRotate();

	glDevice();
	~glDevice();
};

