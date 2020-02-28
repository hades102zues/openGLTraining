#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <vector>


#include <string.h>
#include <stdio.h>
#include <stdlib.h> 

#include <glm/glm.hpp> //standard functions
#include <glm/gtc/matrix_transform.hpp> //special matrix construction
#include <glm/gtc/type_ptr.hpp> //convert our glm matrix to something useable in the shader


#include  "Mesh.h"

//shader variables 
GLuint uniformModel, uniformProjection;

//vertex shader decl.
static const char* vShaderStruct= "												\n\
#version 330																				\n\
																									\n\
out vec4 vColor;																			\n\
																									\n\
layout (location=0)   in vec3 pos;													\n\
uniform mat4 model	;																	\n\
uniform mat4 projection;																\n\
																									\n\
void main() {																					\n\
     gl_Position =  projection * model* vec4(pos, 1.0);					\n\
	 vColor = vec4(clamp(pos, 0.0f, 1.0f),1.0f);								\n\
}";

//fragment shader decl.
static const char* fShaderStruct= "												\n\
#version 330																				\n\
																									\n\
in vec4  vColor; 																			\n\
																									\n\
out vec4 color;																				\n\
																									\n\
void main () {																				\n\
    color = vColor;																			\n\
}";

void createTriangle(std::vector<Mesh*> &list) {

    unsigned int indices[] = {
			0, 3, 1,
		    1, 3, 2,
		    2, 3, 0,
		    0, 1, 2
	};

	GLfloat vertices[]= {
			-1.0f, -1.0f, 0.0f //index 0
			,0.0f, -1.0f, 1.0f // index 1
		    ,1.0f, -1.0f, 0.0f // index 2
		    ,0.0f, 1.0f, 0.0f //index 3
	};

	Mesh* obj1 = new Mesh();
	obj1->createMesh(vertices, indices, 12, 12);
	list.push_back(obj1);
	
}

void addShaders(GLuint &shaderProgram, GLenum shaderType, const char* shaderStruct) {

	GLuint theShader = glCreateShader(shaderType); //create an empty shader object
	
	const GLchar* code[1] ;
	code[0] = shaderStruct;

	GLint length[1]; 
	length[0] = strlen(shaderStruct);


	//(shader, number of incoming shaderStructures, .,.)
	glShaderSource(theShader, 1, code, length);
	glCompileShader(theShader);

	GLint compileStatus;
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		GLchar err[1024] = { 0 };
		glGetShaderInfoLog(theShader, sizeof(err), NULL, err);
		printf(" shader error %s", err);
		return;
	}

	glAttachShader(shaderProgram, theShader);
	

}


void initiateShaderEngine(GLuint &shaderProgram) {

	 shaderProgram =  glCreateProgram(); //create the shader program

	if (!shaderProgram) {
		std::cout << "Shader Program Creation Failed";
		return;
	}

	addShaders(shaderProgram, GL_VERTEX_SHADER, vShaderStruct);
	addShaders(shaderProgram, GL_FRAGMENT_SHADER, fShaderStruct);
	
	GLint linkStatus, validationStatus;

	/**creates the executables on the graphics card**/
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus) {
		GLchar err[1024] = { 0 };
		glGetProgramInfoLog(shaderProgram, sizeof(err), NULL, err);
			printf("Linking error: %s", err);
	}

	/*check that the shader are valid for the current context version*/
	glValidateProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &validationStatus);
	if (!validationStatus) {
		GLchar err[1024] = { 0 };
		glGetProgramInfoLog(shaderProgram, sizeof(err), NULL, err);
		printf("Linking error: %s", err);
	}

	
	uniformModel = glGetUniformLocation(shaderProgram, "model");
	uniformProjection = glGetUniformLocation(shaderProgram, "projection");


	
}


int main(void) {

	const GLint width = 1028, height = 768; //for the window
	GLuint shader;
	 
	std::vector<Mesh*> meshList;

	

	 float toRadians = 3.14159265f  / 180.0f ;
	 float currentAngle = 0.0f;

	 float maxOffset = 0.7f, triOffset = 0.0f, triIncrement = 0.0005f;
	 bool direction = true;
	
	

	/************glfw**************/

	if (!glfwInit()) {
		std::cout << "GLFW Initialization Failed";
		glfwTerminate();
		return -1;
	}
	 //set some window properties
	 
	 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //opengl version support 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //opengl version support 
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //core profile = no backwards compat.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //forwards compatibility ;)


	GLFWwindow* window = glfwCreateWindow(width, height, "Fengine", NULL, NULL);
	if (!window) {
		std::cout << "Window Creation Failed";
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	//grab some buffer info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//tell glew what is the current window it will be drawing to (aka set context)
	glfwMakeContextCurrent(window);

	/***GLEW***/
	glewExperimental= GL_TRUE; //enables extensions. Always set this.

	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "Glew Init Failed";
		return -1;
	}

	//tie opengl to the buffers
	glViewport(0, 0, bufferWidth, bufferHeight);



	/**triangle**/
	createTriangle(meshList);
	initiateShaderEngine(shader);

	glEnable(GL_DEPTH_TEST);

	glm::mat4 matprojection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat) bufferHeight, 0.1f, 100.0f);

	//main loop
	while (!glfwWindowShouldClose(window)) {
		//Get + Handle user inputs
		glfwPollEvents();

		//backdrop between draws
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (direction)
			triOffset += triIncrement;
		else
			triOffset -= triIncrement;

		if (abs(triOffset) >= maxOffset)
			direction = !direction;

		currentAngle += .01f;

		if (currentAngle > 360)
			currentAngle -= 360.0f;
		

		/*draw phase**/

		glUseProgram(shader); //we actually spoil up the execute shader programs we put on the graphics card

	
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate( model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, currentAngle *toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(matprojection));

	
		meshList[0]->renderMesh();
		

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);
		/*_drawphase*/


		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	
	std::cout << "Hello World";
	return 0;
}