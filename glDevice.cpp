#include "glDevice.h"


void glDevice::checkErrorShader(GLuint shader, const GLchar* text, GLuint status)
{
	GLint infoLength = 1;
	GLint messageLength = 0;
	glGetShaderiv(shader, status, &infoLength);
	if (infoLength == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
		GLchar* infoLog = (GLchar*)_malloca(infoLength * sizeof(GLchar));
		glGetShaderInfoLog(shader, infoLength, &messageLength, infoLog);
		printf("%s - %s\n", text, infoLog);
		_freea(infoLog);
	}
}
void glDevice::programInfoLog(GLuint shader)
{
	GLint infoLength = 0;
	GLint lengthWriteInfo = 0;
	GLchar *infoLog;

	glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

	if (infoLength > 0)
	{
		GLchar* infoLog = (GLchar*)_malloca(infoLength * sizeof(GLchar));
		glGetProgramInfoLog(shader, infoLength, &lengthWriteInfo, infoLog);
		printf("Information - %s	\n", infoLog);
		_freea(infoLog);
	}
}

size_t glDevice::push2DTexture(GLubyte4* image, GLuint width, GLuint height) {
	gl_texture_list* nextPtrImageDevice = (gl_texture_list*)calloc(1, sizeof(gl_texture_list));
	if (!headPtrTexturesDevice) {
		headPtrTexturesDevice = nextPtrImageDevice;
		ptrTexturesDevice = nextPtrImageDevice;
	}
	else {
		nextPtrImageDevice->previous = ptrTexturesDevice;
		ptrTexturesDevice->next = nextPtrImageDevice;
		ptrTexturesDevice = nextPtrImageDevice;
	}
	glGenTextures(1, &ptrTexturesDevice->memory_data);
	ptrTexturesDevice->type_memory = GL_TEXTURE_2D;
	glBindTexture(GL_TEXTURE_2D, ptrTexturesDevice->memory_data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (size_t)ptrTexturesDevice;
}

size_t glDevice::push2DTexture(GLubyte* image, GLuint width, GLuint height) {
	gl_texture_list* nextPtrImageDevice = (gl_texture_list*)calloc(1, sizeof(gl_texture_list));
	if (!headPtrTexturesDevice) {
		headPtrTexturesDevice = nextPtrImageDevice;
		ptrTexturesDevice = nextPtrImageDevice;
	}
	else {
		nextPtrImageDevice->previous = ptrTexturesDevice;
		ptrTexturesDevice->next = nextPtrImageDevice;
		ptrTexturesDevice = nextPtrImageDevice;
	}
	glGenTextures(1, &ptrTexturesDevice->memory_data);
	ptrTexturesDevice->type_memory = GL_TEXTURE_2D;
	glBindTexture(GL_TEXTURE_2D, ptrTexturesDevice->memory_data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_R, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return (size_t)ptrTexturesDevice;
}

bool glDevice::pushShaders(GLuint* typeShader, GLchar** code, size_t length)
{
	size_t i = 0;
	GLint infoLength;
	while (*typeShader && length > i) {
		switch (*typeShader) {
		case GL_VERTEX_SHADER: {
			numberVertexShaders++;
			vertexShaders = (GLuint*)realloc(vertexShaders, numberVertexShaders * sizeof(GLuint));
			GLuint* ptrVertexShader = vertexShaders + numberVertexShaders - 1;
			*ptrVertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(*ptrVertexShader, 1, &code[i], NULL);
			glCompileShader(*ptrVertexShader);
			checkErrorShader(*ptrVertexShader, "GL_VERTEX_SHADER", GL_COMPILE_STATUS);
			break;
		}
		case GL_FRAGMENT_SHADER: {
			numberFragmentShaders++;
			fragmentShaders = (GLuint*)realloc(fragmentShaders, numberFragmentShaders * sizeof(GLuint));
			GLuint* ptrFragmentShader = fragmentShaders + numberFragmentShaders - 1;
			*ptrFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(*ptrFragmentShader, 1, &code[i], NULL);
			glCompileShader(*ptrFragmentShader);
			checkErrorShader(*ptrFragmentShader, "GL_FRAGMENT_SHADER", GL_COMPILE_STATUS);
			break;
		}
		case GL_COMPUTE_SHADER: {
			numberComputeShaders++;
			computeShaders = (GLuint*)realloc(computeShaders, numberComputeShaders * sizeof(GLuint));
			GLuint* ptrComputeShader = computeShaders + numberComputeShaders - 1;
			*ptrComputeShader = glCreateShader(GL_COMPUTE_SHADER);
			glShaderSource(*ptrComputeShader, 1, &code[i], NULL);
			glCompileShader(*ptrComputeShader);
			checkErrorShader(*ptrComputeShader, "GL_COMPUTE_SHADER", GL_COMPILE_STATUS);
		}
		default: {
			break;
		}
		}
		typeShader++;
		i++;
	}
	return true;
}
size_t glDevice::pushProgram()
{
	gl_program_list* nextPtrProgramDevice = (gl_program_list*)calloc(1, sizeof(gl_program_list));
	if (!headPtrProgramsDevice) {
		headPtrProgramsDevice = nextPtrProgramDevice;
		ptrProgramsDevice = nextPtrProgramDevice;
	}
	else {
		nextPtrProgramDevice->previous = ptrProgramsDevice;
		ptrProgramsDevice->next = nextPtrProgramDevice;
		ptrProgramsDevice = nextPtrProgramDevice;
	}
	GLint errors;
	ptrProgramsDevice->memory_data = glCreateProgram();
	for (size_t i = 0; i < numberFragmentShaders; i++)
		glAttachShader(ptrProgramsDevice->memory_data, fragmentShaders[i]);
	for (size_t i = 0; i < numberVertexShaders; i++)
		glAttachShader(ptrProgramsDevice->memory_data, vertexShaders[i]);
	for (size_t i = 0; i < numberComputeShaders; i++)
		glAttachShader(ptrProgramsDevice->memory_data, computeShaders[i]);
	glLinkProgram(ptrProgramsDevice->memory_data);
	programInfoLog(ptrProgramsDevice->memory_data);
	for (size_t i = 0; i < numberFragmentShaders; i++)
		glDeleteShader(fragmentShaders[i]);
	for (size_t i = 0; i < numberVertexShaders; i++)
		glDeleteShader(vertexShaders[i]);
	for (size_t i = 0; i < numberComputeShaders; i++)
		glDeleteShader(computeShaders[i]);
	if (numberFragmentShaders)
		free(fragmentShaders);
	if (numberVertexShaders)
		free(vertexShaders);
	if (numberComputeShaders)
		free(computeShaders);
	numberFragmentShaders = 0;
	numberVertexShaders = 0;
	numberComputeShaders = 0;
	fragmentShaders = NULL;
	vertexShaders = NULL;
	computeShaders = NULL;
	return (size_t)ptrProgramsDevice;
}
size_t glDevice::pushVAO(void* data, size_t lengthData, size_t numberCoordinates, GLuint figure)
{
	gl_VAO_list* nextPtrVAODevice = (gl_VAO_list*)calloc(1, sizeof(gl_VAO_list));
	if (!headPtrVAODevice) {
		headPtrVAODevice = nextPtrVAODevice;
		ptrVAODevice = nextPtrVAODevice;
	}
	else {
		nextPtrVAODevice->previous = ptrVAODevice;
		ptrVAODevice->next = nextPtrVAODevice;
		ptrVAODevice = nextPtrVAODevice;
	}
	ptrVAODevice->numberElements = lengthData / numberCoordinates;
	ptrVAODevice->typeFigure = figure;
	glGenVertexArrays(1, &ptrVAODevice->VAO);
	glBindVertexArray(ptrVAODevice->VAO);
	return (size_t)ptrVAODevice;
}

void glDevice::pushBufferInVAO(size_t gl_buffer, size_t gl_VAO, size_t numberCoordinates, GLint attributeLocation)
{
	glBindVertexArray(((gl_VAO_list*)gl_VAO)->VAO);
	glBindBuffer(((gl_buffer_list*)gl_buffer)->type_buffer, ((gl_buffer_list*)gl_buffer)->ptr_buffer);
	glEnableVertexAttribArray(attributeLocation);
	glVertexAttribPointer(attributeLocation, numberCoordinates, GL_FLOAT, GL_FALSE, numberCoordinates * sizeof(GLfloat), (GLvoid*)0);
}
size_t glDevice::pushBuffer(void* data, size_t lengthData, size_t typeBuffer, GLenum renderingMode)
{
	gl_buffer_list* nextPtrBufferDevice = (gl_buffer_list*)calloc(1, sizeof(gl_buffer_list)); 
	if (!headPtrBufferDevice) {
		headPtrBufferDevice = nextPtrBufferDevice;
		ptrBufferDevice = nextPtrBufferDevice;
	}
	else {
		nextPtrBufferDevice->previous = ptrBufferDevice;
		ptrBufferDevice->next = nextPtrBufferDevice;
		ptrBufferDevice = nextPtrBufferDevice;
	}
	glGenBuffers(1, &ptrBufferDevice->ptr_buffer);
	glBindBuffer(typeBuffer, ptrBufferDevice->ptr_buffer);
	glBufferData(typeBuffer, lengthData, data, renderingMode);
	return (size_t)ptrBufferDevice;
}

void glDevice::useProgram(size_t address) {
	glUseProgram(((gl_program_list*)address)->memory_data);
}
void glDevice::useTexture(size_t address) {
	glBindTexture(((gl_texture_list*)address)->type_memory, ((gl_texture_list*)address)->memory_data);
}
void glDevice::useBuffer(size_t address) {
	glBindBuffer(((gl_buffer_list*)address)->type_buffer, ((gl_buffer_list*)address)->ptr_buffer);
}
void glDevice::useVAO(size_t address) {
	glBindVertexArray(((gl_VAO_list*)address)->VAO);
}
GLfloat* glDevice::returnMatrixView() {
	return matrixView.data;
}
GLfloat* glDevice::returnMatrixProjection() {
	return projectionMatrix.data;
}
GLfloat* glDevice::returnMatrixRotate() {
	return matrixRotate.data;
}
void glDevice::setViewport(GLint x, GLint y, GLint width, GLint height) {
	glViewport(x, y, width, height);
}
glDevice::glDevice()
{

	for (size_t i = 0; i < sizeof(*this); i++)
		((GLubyte*)this)[i] = 0;
	projectionMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	matrixView = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	matrixRotate = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	Eye = { 0.0f, 0.0f, 20.0f };
	Center = { 0.0f, 0.0f, 0.0f };
	Up = { 0.0f, 1.0f, 0.0f };
	const GLubyte* name_device =(GLubyte*) glGetString(GL_RENDERER); 
	const GLubyte* version_device = (GLubyte*)glGetString(GL_VERSION);
	printf("--------------Name device--------------\n %s \n", name_device);
	printf("---------------------------------------\n");
	printf("--------------OpenGL version on device--------------\n %s \n", version_device);
	printf("----------------------------------------------------\n ");
	if (glewInit() != GLEW_OK)
	{
		printf("Error glewInit!\n");
		return;
	}
	glewExperimental = GL_TRUE;
	glEnable(GL_DEPTH_TEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH); 
}

void Draw(void* argument) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//glFlush();
}

glDevice::~glDevice()
{

	if (headPtrVAODevice) {
		while (headPtrVAODevice->next != NULL) {
			gl_VAO_list* PtrMemoryDevice = headPtrVAODevice;
			headPtrVAODevice = PtrMemoryDevice->next;
			free(PtrMemoryDevice);
		}
		if (headPtrVAODevice->VAO)
			free(headPtrVAODevice);
	}
	if (headPtrTexturesDevice) {
		while (headPtrTexturesDevice->next != NULL) {
			gl_texture_list* PtrMemoryDevice = headPtrTexturesDevice;
			glBindTexture(PtrMemoryDevice->type_memory, PtrMemoryDevice->memory_data);
			glDeleteTextures(1 , &PtrMemoryDevice->memory_data);
			headPtrTexturesDevice = PtrMemoryDevice->next;
			free(PtrMemoryDevice);
		}
		if (headPtrTexturesDevice->memory_data)
			glBindTexture(headPtrTexturesDevice->type_memory, headPtrTexturesDevice->memory_data),
			glDeleteTextures(1, &headPtrTexturesDevice->memory_data),
			free(headPtrTexturesDevice);
	}
	if (headPtrProgramsDevice) {
		while (headPtrProgramsDevice->next != NULL) {
			gl_program_list* PtrMemoryDevice = headPtrProgramsDevice;
			glDeleteProgram(PtrMemoryDevice->memory_data);
			headPtrProgramsDevice = PtrMemoryDevice->next;
			free(PtrMemoryDevice);
		}
		if (headPtrProgramsDevice->memory_data)
			glDeleteProgram(headPtrProgramsDevice->memory_data),
			free(headPtrProgramsDevice);
	}
	if (headPtrBufferDevice) {
		while (headPtrBufferDevice->next != NULL) {
			gl_buffer_list* PtrMemoryDevice = headPtrBufferDevice;
			glBindBuffer(PtrMemoryDevice->type_buffer, PtrMemoryDevice->ptr_buffer),
			glDeleteBuffers(1, &PtrMemoryDevice->ptr_buffer);
			headPtrBufferDevice = PtrMemoryDevice->next;
			free(PtrMemoryDevice);
		}
		if (headPtrBufferDevice->ptr_buffer)
			glBindBuffer(headPtrBufferDevice->type_buffer, headPtrBufferDevice->ptr_buffer),
			glDeleteBuffers(1, &headPtrBufferDevice->ptr_buffer),
			free(headPtrBufferDevice);
	}
}
