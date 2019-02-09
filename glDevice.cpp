#include "glDevice.h"



glDevice::glDevice()
{
	projectionMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	matrixView = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	matrixRotate = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	Eye = { 0.0f, 0.0f, 20.0f };
	Center = { 0.0f, 0.0f, 0.0f };
	Up = { 0.0f, 1.0f, 0.0f };
	GLubyte* name_device =(GLubyte*) glGetString(GL_RENDERER); 
	GLubyte* version_device = (GLubyte*)glGetString(GL_VERSION);
	printf("--------------Name device--------------\n %s \n", name_device);
	printf("---------------------------------------\n");
	printf("--------------OpenGL version on device--------------\n %s \n", version_device);
	printf("----------------------------------------------------\n ");
	if (glewInit() != GLEW_OK)
	{
		printf("Error glewInit!\n");
		return;
	}
}


glDevice::~glDevice()
{
}
