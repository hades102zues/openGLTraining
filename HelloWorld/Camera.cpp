#include "Camera.h"


Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMouseSensitivity, GLfloat startMovementSpeedSensitivity) {
	
	position = startPosition; //initial starting position of the camera
	worldUp = startUp; // hard coded as (0.0f, 1.0f, 0.0f)
	yaw = startYaw; //hard coded at first, update will the do a recaculation
	pitch = startPitch; //hard coded at first, update will the do a recaculation

	front = glm::vec3(0.0f, 0.0f, -1.0f);  //hard coded at first, update will the do a recaculation
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mouseSensitivity = startMouseSensitivity;
	movementSpeedSensitivity = startMovementSpeedSensitivity;

	update();
	
}

void Camera::keyControl(bool* keyPool, GLfloat deltaTime) {

	GLfloat velocity = deltaTime * movementSpeedSensitivity;

	if (keyPool[GLFW_KEY_W]) {
		position += velocity * front;
	}

	if (keyPool[GLFW_KEY_S]) {
		position -= velocity * front;
	}

	if (keyPool[GLFW_KEY_A]) {
		position -= right * velocity;
	}

	if (keyPool[GLFW_KEY_D]) {
		position += right * velocity;
	}

}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {

	
	xChange *= this->mouseSensitivity;
	yChange *= this->mouseSensitivity;



	yaw += xChange;
	pitch += yChange;

	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update();
}

void Camera::update() {
	

	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);

	
	right = glm::normalize(glm::cross(front, worldUp));// ORDER MATTERS!!
	up = glm::normalize(glm::cross(right, front));// ORDER MATTERS!!


}

glm::mat4 Camera::calculateViewMatrix() {
	
	
	return glm::lookAt(position, front + position, up);

	 
}

glm::vec3 Camera::getCameraPosition() {

	glm::mat4 view = calculateViewMatrix();
	glm::vec4 v4WorldSpaceCamera = glm::inverse(view) * glm::vec4(0.0, 0.0, 0.0, 1.0);
	glm::vec3 v3WorldSpaceCamera = glm::vec3(v4WorldSpaceCamera);

	return v3WorldSpaceCamera;
}