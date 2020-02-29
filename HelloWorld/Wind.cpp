#include "WInd.h"


Wind::Wind() {

	width = 800;
	height = 600;

}

Wind::Wind(GLint windowWidth, GLint windowHeight) {

   width = windowWidth;
   height = windowHeight;

}

int Wind::initialIze() {

	if (!glfwInit()) {
		printf("Glfw Failed to initialize;");
		Wind::~Wind();
		return -1;
	 }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow = glfwCreateWindow(width, height, "Fengine", NULL, NULL);

	if (!mainWindow) {

		printf("GLFW failed to create Window");
		Wind::~Wind();
		return -1;
	}

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental= GL_TRUE;

	if(glewInit() != GLEW_OK)  {

		printf( "Glew Init Failed");
		Wind::~Wind();
		return -1;

	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	glEnable(GL_DEPTH_TEST);


}

bool Wind::shouldWIndowClose() {

	return glfwWindowShouldClose(mainWindow);
}

GLfloat Wind::getBufferWidth() {

	return (GLfloat)bufferWidth;
}

GLfloat Wind::getBufferHeight() {

	return (GLfloat)bufferWidth;
}

void Wind::swapBuffers() {

	glfwSwapBuffers(mainWindow);

}

Wind::~Wind() {
	glfwTerminate();
	if (mainWindow) {
		glfwDestroyWindow(mainWindow);
	}

}