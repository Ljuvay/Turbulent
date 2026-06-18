/*
* Camera.h/.cpp are adapted from the LearnOpenGL textbook
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "camera.h"

// Constructor
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: Position(position), WorldUp(up), yaw(yaw), pitch(pitch),
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), CameraSpeed(SPEED),
	MouseSens(SENSITIVITY)
{
	updateCameraVectors();
}

// Returns the view matrix using LookAt
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

// Set camera perspective
void Camera::setPerspective(float perspective)
{
	camPerspective = perspective;
}

// Return camera perspective
float Camera::getPerspective()
{
	return camPerspective;
}

glm::mat4 Camera::getProjection()
{
	return glm::perspective(glm::radians(this->Fov), getPerspective(), this->minView, this->maxView);
}

// Process keyboard input
void Camera::ProcessInputs(Camera_Movement direction, float deltaTime)
{
	float velocity = CameraSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == UP)
		Position += Up * velocity;
	if (direction == DOWN)
		Position -= Up * velocity;
}

// Process mouse movement
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSens;
	yoffset *= MouseSens;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f) pitch = 89.0f;
		if (pitch < -89.0f) pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::resetRotation() {
	yaw = -90.0;
	pitch = 0.0f;
	updateCameraVectors();
}


// Updates Front, Right, and Up vectors from yaw and pitch
void Camera::updateCameraVectors()
{
	glm::vec3 front(0.0f);
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}