#pragma once


#include <GL/glew.h>
#include <glm/glm.hpp>


class Light { //a more appropriate name for this would have been lighting system.

private:

	//this is more of a scalar than anything -- where we are stating how much of a particular RGB will be utilized. 
	//hence our unit values will be strictly between 0 and 1
	glm::vec3 colour;
	GLfloat ambientIntensity; //a scalar amount as well

	glm::vec3 direction; //should actually be the position of the light. The tutorial guy had messed up  a part of the concept. I fixed up the code to agree with the concept by i am not changing the name...
	GLfloat diffusionIntensity;


public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
			GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffusionIntensityLocation, GLfloat lightDirectionLocation);

	~Light();


};