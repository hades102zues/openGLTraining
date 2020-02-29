#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Wind {

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

public:
	Wind();
	Wind(GLint windowWidth, GLint windowHeight);

	int initialIze();
	bool shouldWIndowClose();

	GLfloat getBufferWidth();
	GLfloat getBufferHeight();

	void swapBuffers();

	
	~Wind();


};
