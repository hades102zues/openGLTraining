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
#include "Shader.h"



static const char* fShaderStruct = "../ShaderStructs/shader.fragment.txt";
static const char* vShaderStruct = "../ShaderStructs/shader.vertex.txt";


void createObjects(std::vector<Mesh*> &list) {

    unsigned int indices[] = {
			0, 3, 1,
		    1, 3, 2,
		    2, 3, 0,
		    0, 1, 2,

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
//

void createShaders(std::vector<Shader*> &list) {

	Shader* shader1 = new Shader();
	shader1->createFromLocation(vShaderStruct, fShaderStruct);

	list.push_back(shader1);
}


int main(void) {

	const GLint width = 1028, height = 768; //for the window
	
	std::vector<Mesh*> meshList;
	std::vector<Shader*> shaderList;
	//shader variables 
	GLuint uniformModel=0, uniformProjection=0;


	

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



	
	createObjects(meshList);
	createShaders(shaderList);


	glEnable(GL_DEPTH_TEST);

	uniformProjection = shaderList[0]->getProjectionLocation();
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

		shaderList[0]->useShader(); //we actually spoil up the execute shader programs we put on the graphics card

		uniformModel = shaderList[0]->getModelLocation();
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