#include "Shader.h"



Shader::Shader() {

	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;

}
void Shader::createFromString(const char* vshaderStruct, const char* fshaderStruct) {

	compileShader(vshaderStruct, fshaderStruct);
}

void Shader::addShader(GLuint& shaderProgram, GLenum shaderType, const char* shaderStruct) {

	GLuint shaderObject = glCreateShader(shaderType);
	const char* code[1];
	code[0] = shaderStruct;

	int length[1];
	length[0] =strlen(shaderStruct);


	glShaderSource(shaderObject, 1, code, length);
	glCompileShader(shaderObject);

	GLint compileStatus;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus) {
		char err[1024] = { 0 };
		glGetShaderInfoLog(shaderObject, sizeof(err), NULL, err);
		printf("Error compiling shader: %s", err);
	}

	glAttachShader(shaderProgram, shaderObject);
}

void Shader::compileShader(const char* vshaderStruct, const char* fshaderStruct) {

	shaderId = glCreateProgram();
	if (!shaderId) {
		printf("Shader Compiler Was not created.");
		return;
	 }

	addShader(shaderId, GL_VERTEX_SHADER, vshaderStruct);
	addShader(shaderId, GL_FRAGMENT_SHADER, fshaderStruct);

	 GLint linkStatusCode;
	glLinkProgram(shaderId);
	glGetProgramiv(shaderId, GL_LINK_STATUS, &linkStatusCode);
	if (!linkStatusCode) {
		char err[1024] = { 0 };
		glGetProgramInfoLog(shaderId, sizeof(err), NULL, err);
		printf(" Error Linking Program. Error: %s", err );
	}

	GLint validateStatusCode;
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &validateStatusCode);
	if (!validateStatusCode) {

		char err[1024] = { 0 };
		glGetProgramInfoLog(shaderId, sizeof(err), NULL, err);
		printf("Error Validating Program. Error: %s", err);
	}

	uniformModel = glGetUniformLocation(shaderId, "uniformModel");
	uniformProjection = glGetUniformLocation(shaderId, "uniformProjection");

}

Shader::~Shader() {

}