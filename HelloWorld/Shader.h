#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

class Shader {

private:
	GLuint
		shaderId,

		uniformModel,
		uniformProjection,
		uniformView,
		uniformCameraPosition,

		uniformAmbientIntensity,
		uniformAmbientColor,
		uniformDiffusionIntensity,
		uniformLightDirection,

		uniformMaterialSpecIntensity,
		uniformMaterialShininess;

	void compileShader(const char* vshaderStruct, const char* fshaderStruct);
	void addShader(GLuint& shaderProgram, GLenum shaderType, const char* shaderStruct);

public:
	Shader();

	void createFromString( const char* vshaderStruct, const char* fshaderStruct);
	void createFromLocation(const char* vshaderStructPath, const char* fshaderStructPath);

	std::string readFile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getCameraPosLocation();

	GLuint getAmbientIntensity();
	GLuint getAmbientColor();
	GLuint getDiffusionIntensityLocation();
	GLuint getLightDirectionLocation();

	GLuint getMaterialSpecIntLocation();
	GLuint getMaterialShineLocation();


	

	void useShader();
	void clearShader();


	~Shader();


};