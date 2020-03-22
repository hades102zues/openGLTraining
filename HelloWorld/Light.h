#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>


class Light {

private:

	//this is more of a scalar than anything -- where we are stating how much of a particular RGB will be utilized. 
	//hence our unit values will be strictly between 0 and 1
	glm::vec3 colour;

	GLfloat ambientIntensity; //a scalar amount as well


public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

	void useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);

	~Light();


};