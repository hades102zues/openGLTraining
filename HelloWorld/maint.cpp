#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>


static const char* vShaderStruct = "									\n\
#version 330																	\n\
																						\n\
layout (location=0) in vec3 pos;										\n\
																						\n\
void main() {																		\n\
																						\n\
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);				    \n\
}																							\n\
";

static const char* fShaderStruct = "								\n\
#version 330																		\n\
																							\n\
out vec4 color;																	\n\
																							\n\
void main() {																			\n\
	color = vec4(0.3, 0, 0.8, 1.0);																	\n\
}																							\n\
";



void bufferWorld(GLuint &VAO, GLuint &VBO) {

	GLfloat triangle[] = {
		-1.0f,  -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void compileShaders(GLuint& shaderProgram, GLenum shaderType, const char* shaderStruct) {
	GLuint shader = glCreateShader(shaderType); 

	const char* theCode[1];
	theCode[0] = shaderStruct;

	int codeLength[1];
	codeLength[0] = strlen(shaderStruct);

	glShaderSource(shader,1, theCode, codeLength );
	glCompileShader(shader);

	GLint compileStatusCode;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatusCode);
	if (!compileStatusCode) {
		GLchar err[1024] = { 0 };
		glGetShaderInfoLog(shader, sizeof(err), NULL, err);
		printf(" shader error %s", err);
		return;
	}

	glAttachShader(shaderProgram, shader);

};


void createShaderExes(GLuint &shaderProgram) {

	shaderProgram = glCreateProgram();

	if (!shaderProgram) {
		printf("Shader Program Failed creation");
		return;
	}

	compileShaders(shaderProgram, GL_VERTEX_SHADER, vShaderStruct);
	compileShaders(shaderProgram, GL_FRAGMENT_SHADER, fShaderStruct);

	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	GLint linkStatus, validationStatus;

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &validationStatus);

	if (!linkStatus) {
		printf("Error Linking Shader Program");
		return;
	}

	if (!validationStatus) {
		printf("Error validation shader Program");
		return;
	}
	//
}



int main1() {

	const GLint width = 1024, height = 768;
	GLuint vao, vbo, shader;

	if (!glfwInit()) {
		printf("GLFW Init Failed");
		glfwTerminate();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(width, height, "LEARN", NULL, NULL);
	if (!mainWindow) {
		printf("Window creation failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
   }

	glfwMakeContextCurrent(mainWindow);

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);


	if (glewInit() != GLEW_OK) {
		printf("Glew init failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return -1;
	}
	glewExperimental = GL_TRUE;

	glViewport(0,0,bufferWidth,bufferHeight);

	bufferWorld(vao, vbo);
	createShaderExes(shader);

	while (!glfwWindowShouldClose(mainWindow)) {
		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	



	return 0;
}