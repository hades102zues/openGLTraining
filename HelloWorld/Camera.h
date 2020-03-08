#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
    
private:
    glm::vec3 position; //world position
    glm::vec3 front; //used to always ensure that the camera is pointing at some area in front of the lens
    glm::vec3 up;
    glm::vec3 right;

    glm::vec3 worldUp;

    GLfloat pitch; //rotation around x-axis
    GLfloat yaw; //rotation around y-axis

public :
    Camera();
    ~Camera();
};
