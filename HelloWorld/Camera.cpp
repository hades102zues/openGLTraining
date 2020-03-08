#include "Camera.h"


Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMouseSensitivity, GLfloat startMovementSpeedSensitivity) {
	
	position = startPosition;
	up = startUp; 
	yaw = startYaw;
	pitch = startPitch;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	mouseSensitivity = startMouseSensitivity;
	movementSpeedSensitivity = startMovementSpeedSensitivity;

	update();
	
}

void Camera::update() {
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	front = glm::normalize(front);

	//camera direction = front + camera's position
	glm::vec3 direction = front + position;
	right = glm::normalize(glm::cross(worldUp, direction));// ORDER MATTERS!!
	up = glm::normalize(glm::cross(direction, right));// ORDER MATTERS!!



}