#include "Light.h"


Light::Light() {

	colour = glm::vec3(1.0f, 1.0f, 1.0f); 
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffusionIntensity = 0.0f;

}


Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity,
					GLfloat xDirection, GLfloat yDirection, GLfloat zDirection, GLfloat dIntensity) {

	colour = glm::vec3(red, green, blue);
	ambientIntensity =  aIntensity;

	direction = glm::vec3(xDirection, yDirection, zDirection);
	diffusionIntensity = dIntensity;

}

void Light::useLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffusionIntensityLocation, GLfloat lightDirectionLocation) {

	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform1f(diffusionIntensityLocation, diffusionIntensity);
	glUniform3f(lightDirectionLocation, direction.x, direction.y, direction.z);

}


Light::~Light() {

}