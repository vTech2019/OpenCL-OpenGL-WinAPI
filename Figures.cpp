#include "Figures.h"

//volatile size_t* MakeFigures::makeField(OpenGL_API::GraphicsOpenGL* mainGL, float x_near, float x_far, float y_near, float y_far, float z_near, float z_far, float x_step, float y_step, float z_step) {
//
//	volatile size_t size_x = (x_far - x_near) / x_step;
//	volatile size_t size_y = (y_far - y_near) / y_step;
//	volatile size_t size_z = (z_far - z_near) / z_step;
//
//	size_t size_vertex = size_x * size_y * size_z * 3;
//	size_t size_texture = (size_vertex * 2) / 3;
//	size_t size_index = size_x * size_y * 4;
//	GLfloat* position = (GLfloat*)malloc(size_vertex * sizeof(GLfloat));
//	GLfloat* normal = (GLfloat*)malloc(size_vertex * sizeof(GLfloat));
//	GLfloat* color = (GLfloat*)malloc(size_vertex * sizeof(GLfloat));
//	GLuint* index = (GLuint*)malloc(size_index * sizeof(GLuint));
//	GLfloat* texture = (GLfloat*)malloc(size_texture * sizeof(GLfloat));
//	size_t width_height = size_y * size_x * 3;
//	size_t width = size_x * 3;
//
//	//GLfloat x_vector = size_x / sqrt(size_x * size_x + size_y * size_y + size_z * size_z);
//	//GLfloat y_vector = size_y / sqrt(size_x * size_x + size_y * size_y + size_z * size_z);
//	//GLfloat z_vector = size_z / sqrt(size_x * size_x + size_y * size_y + size_z * size_z);
//	GLfloat x_vector = 0;
//	GLfloat y_vector = 0;
//	GLfloat z_vector = 1;
//
//	for (size_t z = 0, texture_index = 0; z < size_z; z++) {
//		for (size_t y = 0; y < size_y; y++) {
//			for (size_t x = 0, i = 0; x < size_x; x++) {
//				position[z * width_height + y * width + i] = x_near + x * x_step;
//				position[z * width_height + y * width + i + 1] = y_near + y * y_step;
//				position[z * width_height + y * width + i + 2] = z_near + z * z_step;
//
//				normal[z * width_height + y * width + i] = x_vector;
//				normal[z * width_height + y * width + i + 1] = y_vector;
//				normal[z * width_height + y * width + i + 2] = z_vector;
//
//				color[z * width_height + y * width + i] = 1.0f;
//				color[z * width_height + y * width + i + 1] = 1.0f;
//				color[z * width_height + y * width + i + 2] = 1.0f;
//				texture[texture_index++] = (float)x / size_x;
//				texture[texture_index++] = (float)y / size_y;
//				i += 3;
//			}
//		}
//	}
//	width_height /= 3;
//	width /= 3;
//	size_t x = 0, y = 0, j = 0;
//	for (size_t k = 0; k < size_y - 1; k++) {
//		if (k % 2 == 0) {
//			for (size_t i = 0; i < size_x; i++) {
//				index[j++] = size_x * x++ + y;
//				index[j++] = size_x * x-- + y++;
//			}
//			y--;
//			x++;
//		}
//		else {
//			for (size_t i = 0; i < size_x; i++) {
//				index[j++] = size_x * x++ + y;
//				index[j++] = size_x * x-- + y--;
//			}
//			x++;
//			y++;
//		}
//	}
//	//
//	mainGL->pushBuffer(position, size_vertex * sizeof(GLfloat), 3, GL_TRIANGLE_STRIP, glGetAttribLocation(mainGL->getProgram(0), "positions"), true, GL_ARRAY_BUFFER_ARB, GL_DYNAMIC_DRAW);
//	mainGL->pushBuffer(color, size_vertex * sizeof(GLfloat), 3, NULL, glGetAttribLocation(mainGL->getProgram(0), "colors"), false, GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW);
//	mainGL->pushBuffer(normal, size_vertex * sizeof(GLfloat), 3, NULL, glGetAttribLocation(mainGL->getProgram(0), "normals"), false, GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW);
//	mainGL->pushBuffer(texture, size_texture * sizeof(GLuint), 2, NULL, glGetAttribLocation(mainGL->getProgram(0), "texture"), false, GL_ARRAY_BUFFER_ARB, GL_STATIC_DRAW);
//	mainGL->pushBuffer(index, (j) * sizeof(GLuint), 2, NULL, -1, false, GL_ELEMENT_ARRAY_BUFFER_ARB, GL_STATIC_DRAW);
//	free(position);
//	free(normal);
//	free(color);
//	free(index);
//	free(texture);
//	volatile size_t work[2] = { size_x, size_y };
//	return work;
//}

Figures::Figures()
{
}


Figures::~Figures()
{
}
