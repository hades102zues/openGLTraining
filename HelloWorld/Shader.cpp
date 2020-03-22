#include "Shader.h"



Shader::Shader() {

	shaderId = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;

}
void Shader::createFromString(const char* vshaderStruct, const char* fshaderStruct) {

	compileShader(vshaderStruct, fshaderStruct);
}
void Shader::createFromLocation(const char* vshaderStructPath, const char* fshaderStructPath) {
	std::string vShaderStruct = readFile(vshaderStructPath);
	std::string fShaderStruct = readFile(fshaderStructPath);

	const char* vertexCode = vShaderStruct.c_str();
	const char* fragmentCode = fShaderStruct.c_str();

	compileShader(vertexCode, fragmentCode);
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
		return;
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
		GLchar err[1024] = { 0 };
		glGetProgramInfoLog(shaderId, sizeof(err), NULL, err);
		printf(" Error Linking Program. Error: %s", err );
		return;
	}

	GLint validateStatusCode;
	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &validateStatusCode);
	
	if (!validateStatusCode) {

		GLchar err[1024] = { 0 };
		glGetProgramInfoLog(shaderId, sizeof(err), NULL, err);
		printf("Error Validating Program. Error: %s", err);
		return;
	}

	uniformModel = glGetUniformLocation(shaderId, "model");
	uniformProjection = glGetUniformLocation(shaderId, "projection");
	uniformView = glGetUniformLocation(shaderId, "view");
	uniformAmbientColor = glGetUniformLocation(shaderId, "directionalLight.color");
	uniformAmbientIntensity = glGetUniformLocation(shaderId, "directionalLight.ambientIntensity");

	
}

std::string Shader::readFile( const char* fileLocation) {

	std::string content;

	std::ifstream filestream(fileLocation, std::ios::in);

	if (!filestream.is_open()) {
		printf("Error retrieving shader structures from file \n");
		return "";
	}

	std::string line;
	while (!filestream.eof()) {

		std::getline(filestream, line);
		content.append(line + "\n"); //if u don't put that newline then glsl will explode


	}

	filestream.close();
	return content;

}

GLuint Shader::getProjectionLocation() {
	return uniformProjection;
}
GLuint Shader::getModelLocation() {
	return uniformModel;
}

GLuint Shader::getViewLocation() {
	return uniformView;
}

GLuint Shader::getAmbientColor() {
	return uniformAmbientColor;
}

GLuint Shader::getAmbientIntensity() {
	return uniformAmbientIntensity;
}


void Shader::useShader() {
	glUseProgram(shaderId);
}



void Shader::clearShader() {
	
	if (shaderId != 0) {
		glDeleteProgram(shaderId);
		shaderId = 0;
	 }

	uniformModel = 0;
	uniformProjection = 0;

}



Shader::~Shader() {
	clearShader();
}