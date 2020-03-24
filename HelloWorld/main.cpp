#define STB_IMAGE_IMPLEMENTATION //only way to use the stb_image.h properly

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
#include "Texture.h"
#include "Light.h"



static const char* fShaderStruct = "../ShaderStructs/shader.fragment.txt";
static const char* vShaderStruct = "../ShaderStructs/shader.vertex.txt";



void calcAverageNormal(unsigned int* indices, unsigned int indicesCount, GLfloat * vertices, unsigned int verticeCount, unsigned int vLengthOffset, unsigned int normalInlineOffset) {

	for (size_t i = 0; i < indicesCount; i += 3) {		//will go to first element of  each line of the indices[]

		//these three variables will effectively take on the index of some X-coord,  each on different lines, thrice consecutive order
		// for e.g indices[1] = 3 * vlengthOffset = 8 would take us to  index 24 of vertices.
		//tis a very cool way to navigate
		unsigned int in0 = indices[ i ] * vLengthOffset;
		unsigned int in1 = indices[ i + 1 ] * vLengthOffset; 
		unsigned int in2 = indices[ i + 2 ] * vLengthOffset; 


		//next we produce a vector that would connect each pair of vertices
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1+1] - vertices[in0+1], vertices[in1+2] -vertices[in0+2]);
		glm::vec3 v2(vertices[in2] - vertices[in1], vertices[in2+1] - vertices[in1 + 1], vertices[in2+2] - vertices[in1 + 2]);

		//hence v1 ' v2 will form a plane which we can then find a normal from
		glm::vec3 normal = glm::normalize( glm::cross(v1,v2) );


		//now update the normals coordinates of each vertex we used above. Take note 
		//that in updating we will add. this has a nice averaging effect on the normal and smoothes out our light
		vertices[in0 + normalInlineOffset] += normal.x;	vertices[in0 + normalInlineOffset + 1] += normal.y; vertices[in0 + normalInlineOffset + 2] += normal.z;
		vertices[in1 + normalInlineOffset] += normal.x; vertices[in1 + normalInlineOffset + 1] += normal.y; vertices[in1 + normalInlineOffset + 2] += normal.z;
		vertices[in2 + normalInlineOffset] += normal.x; vertices[in2 + normalInlineOffset + 1] += normal.y; vertices[in2 + normalInlineOffset + 2] += normal.z;
		

	}

}

void createObjects(std::vector<Mesh*> &list) {

    unsigned int indices[] = {
			0, 3, 1,
	        1, 3, 2,
		    2, 3, 0,
		    0, 1, 2,
			
	};


	GLfloat vertices[]= {
		//*** x,       y,       z,       u,       v				nx				ny				nz
			-1.0f , -1.0f,  0.0f,    0.0f,     0.0f,			0.0f,			0.0f,        0.0f,			//index 0
			0.0f, -1.0f,   1.0f,    0.5f,	    0.0f,			0.0f,			0.0f,        0.0f,			// index 1
		    1.0f, -1.0f,   0.0f ,   1.0f,      0.0f,			0.0f,			0.0f,        0.0f,			// index 2
		    0.0f, 1.0f,    0.0f,    0.5f,      1.0f,			0.0f,			0.0f,        0.0f			//index 3
		   

		
	};


	Mesh* obj1 = new Mesh();
	obj1->createMesh(vertices, indices, 20, 12);
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
			-90.0f,
			0.0f,
			0.5,
			1.0f
		);

	Light* mainLight = new Light(1.0f, 1.0f, 1.0f, 0.1005f);

	//Textures
	Texture* brickTexture = new Texture("./Textures/brick.png");
	Texture* dirtTexture = new Texture("./Textures/dirt.png");

	brickTexture->loadTexture();
	dirtTexture->loadTexture();

	//shader variables 
	GLuint 
		uniformModel=0, 
		uniformProjection=0, 
		uniformView = 0,
		uniformAmbientColor =0,
		uniformAmbientIntensity = 0;

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0;


	 float toRadians = 3.14159265f  / 180.0f ;
	 float currentAngle = 0.0f;

	 float maxOffset = 0.7f, triOffset = 0.0f, triIncrement = 0.0005f;
	 bool direction = true;

	
	createObjects(meshList);
	createShaders(shaderList);


	glm::mat4 matprojection = glm::perspective(45.0f, programWindow->getBufferWidth() / programWindow->getBufferHeight(), 0.1f, 100.0f);

	//main loop
	while (!programWindow->shouldWIndowClose()) {

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;


		//Get + Handle user inputs
		glfwPollEvents();

		
		//these should also come after the poll event
		camera->keyControl(programWindow->getKeyPool(), deltaTime);
		camera->mouseControl(programWindow->getXChange(), programWindow->getYChange());

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
		uniformAmbientColor = shaderList[0]->getAmbientColor();
		uniformAmbientIntensity = shaderList[0]->getAmbientIntensity();

		mainLight->useLight(uniformAmbientIntensity, uniformAmbientColor);
		
		glm::mat4 model = glm::mat4(1.0); //a fresh identity matrix.
		model = glm::translate( model, glm::vec3(0.0f, 0.0f, -4.0f));
		//model = glm::rotate(model, currentAngle *toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(matprojection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
	   

		brickTexture->useTexture();
		meshList[0]->renderMesh();
		

		glUseProgram(0);
		/*_drawphase*/


		programWindow->swapBuffers();
	}

	programWindow->~Wind();

	return 0;
}