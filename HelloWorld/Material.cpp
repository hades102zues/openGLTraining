#include "Material.h"



Material::Material() {
	specIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
	specIntensity = sIntensity;
	shininess = shine;
}

void Material::useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation) {
	glUniform1f(specularIntensityLocation, specIntensity);
	glUniform1f(shininessLocation, shininess);
}