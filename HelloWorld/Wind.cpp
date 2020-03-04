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
	//set the window event listeners
	createCallbacks();

	glewExperimental= GL_TRUE;

	if(glewInit() != GLEW_OK)  {

		printf( "Glew Init Failed");
		Wind::~Wind();
		return -1;

	}

	glViewport(0, 0, bufferWidth, bufferHeight);
	glEnable(GL_DEPTH_TEST);


	//storing the context of the Wind class within in the window for extraction in the key_callback
	glfwSetWindowUserPointer(mainWindow, this);
	
	

}

void Wind::createCallbacks() {

	glfwSetKeyCallback(mainWindow, key_callback);

}

void Wind::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	//*A Quick Unrelated Lesson*
	//Functions descend from the C language. Classes descend from C++ having both an address and a instance/context given by the this pointer
	//C functions only receives the address of the object with no included binding to the specific instance of the object. 
	//We need the context in order to perform window->anything or the compiler will SCREAM !
	//long story short, this C function is not fit enough to handle C++ structure in an intuitive way like we'd expect from something like javascript.
   

	//going from a void* pointer* to a Wind*
	Wind* localWind = static_cast<Wind*> (glfwGetWindowUserPointer(window));
	                                                               

	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {

		glfwSetWindowShouldClose(window, GL_TRUE);
		
	}
		
	if (key >= 0 && key <1024 ) {

		if (action == GLFW_PRESS) {
			localWind->keyPool[key] = true;
			printf("key pressed: %d \n", key);
		}

		else if (action == GLFW_RELEASE) {
			localWind->keyPool[key] = false;
			printf("key released: %d \n", key);
		}

	}
	    


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