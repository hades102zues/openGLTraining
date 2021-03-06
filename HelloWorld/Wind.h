#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Wind {

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	//this array will help keep track of what keys have been pressed; 0 to 1023 represends the unicode number associated with each key.
		bool keyPool[1024];  

   //values necesary to keep track of mouse movements
		GLfloat lastX;
		GLfloat lastY;
		GLfloat xChange; //this players get passed to the camera; used in yaw calculation
		GLfloat yChange; //this gets passed to the camera; pitch calculation
		bool mouseFirstMove; //prevents camera jump on initial mouse movement

		void createCallbacks();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_callback(GLFWwindow* window, double currX, double currY);

public:
	Wind();
	Wind(GLint windowWidth, GLint windowHeight);

	int initialIze();
	bool shouldWIndowClose();

	bool* getKeyPool() { return keyPool; };
	GLfloat getXChange();
	GLfloat getYChange();

	GLfloat getBufferWidth();
	GLfloat getBufferHeight();
	

	void swapBuffers();

	
	~Wind();


};
