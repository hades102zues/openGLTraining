#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera {
    
private:
    glm::vec3 position; //world position
    glm::vec3 front; //used to always ensure that the camera is pointing at some area in front of the lens
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 worldUp;

    GLfloat pitch; //rotation around y-axis; The pitch will get the yChange of the mouse
    GLfloat yaw; //rotation around x-axis; The yaw will get the xChange of the mouse

    GLfloat mouseSensitivity;
    GLfloat movementSpeedSensitivity; //for the keyboard

    

public :
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMouseSensitivity, GLfloat startMovementSpeedSensitivity);
    void keyControl(bool* keyPool);
    void  update();
    glm::mat4 calculateViewMatrix();
    ~Camera();
};
