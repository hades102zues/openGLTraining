#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

class Shader {

private:
	GLuint shaderId, uniformModel, uniformProjection;

	void compileShader(const char* vshaderStruct, const char* fshaderStruct);
	void addShader(GLuint& shaderProgram, GLenum shaderType, const char* shaderStruct);

public:
	Shader();

	void createFromString( const char* vshaderStruct, const char* fshaderStruct);

	GLuint getProjectionLocation();
	GLuint getModelLocation();

	void useShader();
	void clearShader();


	~Shader();


};