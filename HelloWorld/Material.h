#pragma once

#include <GL/glew.h>


class Material {

private :
	GLfloat specIntensity;
	GLfloat shininess; //angle influenced

public :

	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void useMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

	~Material();

};
