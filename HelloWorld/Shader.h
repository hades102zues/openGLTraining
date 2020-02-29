#pragma once

#include <GL/glew.h>
#include <stdio.h>
#include <string>
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
	void createFromLocation(const char* vshaderStructPath, const char* fshaderStructPath);

	std::string readFile(const char* fileLocation);

	GLuint getProjectionLocation();
	GLuint getModelLocation();

	void useShader();
	void clearShader();


	~Shader();


};