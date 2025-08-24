#ifndef CAMERA_H
#define CAMERA_H

#include "../../vendor/glad/include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  4.0f;
const float SENSITIVITY =  0.03f;
const float ZOOM        =  45.0f;

class Camera {
public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    float Pitch = 0.0f;
    float Yaw = -90.0f;
    // Camera Options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with Vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix(){
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboard(Camera_Movement direction, float deltaTime){
        float cameraSpeed = 3.5f * deltaTime;

        if(direction == FORWARD){
            Position += cameraSpeed * Front;
        }
        if(direction == BACKWARD){
            Position -= cameraSpeed * Front;
        }
        if(direction == LEFT){
            Position -= glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        }
        if(direction == RIGHT){
            Position += glm::normalize(glm::cross(Front, Up)) * cameraSpeed;
        }

        Position.y = 3.0f;
    }

    void ProcessMouseScroll(float yoffset){
        Zoom -= (float)yoffset;
        Zoom = glm::clamp(Zoom, 1.0f, 50.0f);
    }

    void ProcessMouseMovement(float xoffset, float yoffset){
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // lo: -89, hi: 89 [so that we don't have to reverse the scene]
        Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

        updateCameraVectors();
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors(){
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
