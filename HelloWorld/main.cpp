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


#include  "Mesh.h" //system that handles the data for object construction
#include "Shader.h" //program for compiling the shaders
#include "Wind.h" //our system window
#include "Camera.h"



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

	Wind* programWindow = new Wind(800, 600); 
	programWindow->initialIze();
	
	std::vector<Mesh*> meshList; //idea here is that you have an array that keeps track of all the objects you will create
	std::vector<Shader*> shaderList; //idea here is that you an array that keeps track of  all the shader Programs you will create

	Camera* camera = new 
		Camera(
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			0.0f,
			0.0f,
			5.0f,
			1.0f
		);

	//shader variables 
	GLuint uniformModel=0, uniformProjection=0, uniformView = 0;


	 float toRadians = 3.14159265f  / 180.0f ;
	 float currentAngle = 0.0f;

	 float maxOffset = 0.7f, triOffset = 0.0f, triIncrement = 0.0005f;
	 bool direction = true;

	
	createObjects(meshList);
	createShaders(shaderList);


	glm::mat4 matprojection = glm::perspective(45.0f, programWindow->getBufferWidth() / programWindow->getBufferHeight(), 0.1f, 100.0f);

	//main loop
	while (!programWindow->shouldWIndowClose()) {

		//Get + Handle user inputs
		glfwPollEvents();

		camera->keyControl(programWindow->getKeyPool());

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
		uniformProjection = shaderList[0]->getProjectionLocation();
		uniformView = shaderList[0]->getViewLocation();
		
		glm::mat4 model = glm::mat4(1.0); //a fresh identity matrix.
		model = glm::translate( model, glm::vec3(0.0f, 0.0f, -4.0f));
		model = glm::rotate(model, currentAngle *toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(matprojection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	   
		meshList[0]->renderMesh();
		

		glUseProgram(0);
		/*_drawphase*/


		programWindow->swapBuffers();
	}

	programWindow->~Wind();

	return 0;
}