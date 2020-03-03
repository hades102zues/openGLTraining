#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Wind {

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	int keyPool[1024];  //this array will help keep track of what keys have been pressed; 0 to 1023 represends the unicode number associated with each key.

public:
	Wind();
	Wind(GLint windowWidth, GLint windowHeight);

	int initialIze();
	bool shouldWIndowClose();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	GLfloat getBufferWidth();
	GLfloat getBufferHeight();

	void swapBuffers();

	
	~Wind();


};
