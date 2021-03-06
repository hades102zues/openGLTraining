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
#include "Material.h"


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
		//we make in0 the arbitrary origin of the surface
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1+1] - vertices[in0+1], vertices[in1+2] -vertices[in0+2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2+1] - vertices[in0 + 1], vertices[in2+2] - vertices[in0 + 2]);

		//hence v2, v1 will form a plane which we can then find a normal from.
		//using the right hand twist rule, which is founded upon the positive y-axis going upwards
		//and the positive x-axis going to the right,
		//curve our hand in such a fashion that the thumb  points in the positive z direction (because we want the light to point out of the shape and not into it).
		//the vector at the tip of the fingers shall become the second item in the cross-product
		glm::vec3 normal = glm::normalize( glm::cross(v2,v1) );


		//now update the normals coordinates of each vertex we used above. Take note 
		//that in updating we will add. we will be averaging the values later on
		vertices[in0 + normalInlineOffset] += normal.x;	vertices[in0 + normalInlineOffset + 1] += normal.y; vertices[in0 + normalInlineOffset + 2] += normal.z;
		vertices[in1 + normalInlineOffset] += normal.x; vertices[in1 + normalInlineOffset + 1] += normal.y; vertices[in1 + normalInlineOffset + 2] += normal.z;
		vertices[in2 + normalInlineOffset] += normal.x; vertices[in2 + normalInlineOffset + 1] += normal.y; vertices[in2 + normalInlineOffset + 2] += normal.z;
		

	}

	for (size_t i = 0;  i < verticeCount / vLengthOffset; i++) { 


		
		// i * vLengthOffset will take us to the first item of each line, of our vertices[]

		unsigned int nFirst = vertices[(i * vLengthOffset) + normalInlineOffset];//will get the index of the x coord of the normal

		glm::vec3 vec(vertices[nFirst], vertices[nFirst + 1], vertices[nFirst + 2]); //pull the normal
		
		vec = glm::normalize(vec);

		vertices[nFirst] = vec.x; vertices[nFirst + 1] = vec.y; vertices[nFirst + 2] = vec.z;


		//Funny enough we don't actually need to find the average just normalize the sum. don't ask me why. it is what it is.
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
			-1.0f , -1.0f,  -0.6f,    0.0f,     0.0f,			0.0f,			0.0f,        0.0f,			//index 0
			0.0f, -1.0f,   1.0f,    0.5f,	    0.0f,			0.0f,			0.0f,        0.0f,			// index 1
		    1.0f, -1.0f,   -0.6f ,   1.0f,      0.0f,			0.0f,			0.0f,        0.0f,			// index 2
		    0.0f, 1.0f,    0.0f,    0.5f,      1.0f,			0.0f,			0.0f,        0.0f			//index 3
		   

		
	};

	calcAverageNormal(indices, 12, vertices, 32, 8, 5);


	Mesh* obj1 = new Mesh();
	obj1->createMesh(vertices, indices, 32, 12);
	list.push_back(obj1);
	
}
//

void createShaders(std::vector<Shader*> &list) {

	Shader* shader1 = new Shader();
	shader1->createFromLocation(vShaderStruct, fShaderStruct);

	list.push_back(shader1);
}


int main(void) {

	Wind* programWindow = new Wind(1366, 768); 
	programWindow->initialIze();
	
	std::vector<Mesh*> meshList; //idea here is that you have an array that keeps track of all the objects you will create
	std::vector<Shader*> shaderList; //idea here is that you an array that keeps track of  all the shader Programs you will create

	Camera* camera = new 
		Camera(
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			-90.0f,
			0.0f,
			0.5,
			1.0f
		);

	Light* mainLight = new Light(1.0f, 1.0f, 1.0f, 0.1005f,
													7.0f,1.0f,0.0f,0.101f);

	//Textures
	Texture* brickTexture = new Texture("./Textures/brick.png");
	Texture* dirtTexture = new Texture("./Textures/dirt.png");

	brickTexture->loadTexture();
	dirtTexture->loadTexture();

	//Material
	Material* shinnyMaterial = new Material(1.5f, 32.0f);


	//shader variables 
	GLuint 
		uniformModel=0, 
		uniformProjection=0, 
		uniformView = 0,
		uniformEyePos = 0,

		uniformAmbientColor =0,
		uniformAmbientIntensity = 0,
		uniformLightDirection = 0,
		uniformDiffusionIntensity = 0,
		
		uniformMaterialSpecIntensity = 0,
		uniformMaterialShine;

	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0;


	 float toRadians = 3.14159265f  / 180.0f ;
	 float currentAngle = 0.0f;

	 float maxOffset = 0.7f, triOffset = 0.0f, triIncrement = 0.005f;
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

		currentAngle += .1f;

		if (currentAngle > 360)
			currentAngle -= 360.0f;
		

		/*draw phase**/

		shaderList[0]->useShader(); //we actually spoil up the execute shader programs we put on the graphics card
		uniformModel = shaderList[0]->getModelLocation();
		uniformProjection = shaderList[0]->getProjectionLocation();
		uniformView = shaderList[0]->getViewLocation();
		uniformEyePos = shaderList[0]->getCameraPosLocation();

		uniformAmbientColor = shaderList[0]->getAmbientColor();
		uniformAmbientIntensity = shaderList[0]->getAmbientIntensity();
		uniformLightDirection = shaderList[0]->getLightDirectionLocation();
		uniformDiffusionIntensity = shaderList[0]->getDiffusionIntensityLocation();

		uniformMaterialSpecIntensity = shaderList[0]->getMaterialSpecIntLocation();
		uniformMaterialShine = shaderList[0]->getMaterialShineLocation();


		mainLight->useLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffusionIntensity, uniformLightDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(matprojection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		glUniform3f(uniformEyePos, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);
		
		glm::mat4 model = glm::mat4(1.0); //a fresh identity matrix.
		model = glm::translate( model, glm::vec3(0.0f, -1.0f, -5.0f));
		//model = glm::rotate(model, currentAngle *toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
	  
		brickTexture->useTexture();
		shinnyMaterial->useMaterial(uniformMaterialSpecIntensity, uniformMaterialShine);
		meshList[0]->renderMesh();
		

		glUseProgram(0);
		/*_drawphase*/


		programWindow->swapBuffers();
	}

	programWindow->~Wind();

	return 0;
}