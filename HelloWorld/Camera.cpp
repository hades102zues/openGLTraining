#include "Camera.h"


Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMouseSensitivity, GLfloat startMovementSpeedSensitivity) {
	
	position = startPosition;
	worldUp = startUp; 
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mouseSensitivity = startMouseSensitivity;
	movementSpeedSensitivity = startMovementSpeedSensitivity;

	update();
	
}

void Camera::keyControl(bool* keyPool) {

	if (keyPool[GLFW_KEY_W]) {
		position += movementSpeedSensitivity * front;
	}

	if (keyPool[GLFW_KEY_S]) {
		position -= movementSpeedSensitivity * front;
	}

	if (keyPool[GLFW_KEY_A]) {
		position -= right * movementSpeedSensitivity;
	}

	if (keyPool[GLFW_KEY_D]) {
		position += right * movementSpeedSensitivity;
	}

}

void Camera::update() {
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);

	//camera's direction = front + position
	glm::vec3 direction = front + position;
	right = glm::normalize(glm::cross(front, worldUp));// ORDER MATTERS!!
	up = glm::normalize(glm::cross(right, front));// ORDER MATTERS!!


}

glm::mat4 Camera::calculateViewMatrix() {
	
	
	return glm::lookAt(position, front + position, up);

	 
}