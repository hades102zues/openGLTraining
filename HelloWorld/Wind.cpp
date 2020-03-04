#include "WInd.h"


Wind::Wind() {

	width = 800;
	height = 600;
	bufferWidth = bufferHeight = 0;
	mainWindow = nullptr;
	
	for (int i = 0; i < 1024; i++) {
		keyPool[i] = 0;
	}
	
}

Wind::Wind(GLint windowWidth, GLint windowHeight) {

   width = windowWidth;
   height = windowHeight;
   mainWindow = nullptr;
   bufferWidth = bufferHeight = 0;

   for (int i = 0; i < 1024; i++) {
	   keyPool[i] = 0;
   }

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
	glfwSetKeyCallback(mainWindow, key_callback);
	glEnable(GL_DEPTH_TEST);


}

void Wind::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, GL_TRUE);

	


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