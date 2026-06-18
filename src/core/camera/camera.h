#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 50.0f;
const float SENSITIVITY = 0.1f;
const float FOV = 70.0f;
const float MIN_VIEW = 0.1f;
const float MAX_VIEW = 1000.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float yaw;
	float pitch;

	float CameraSpeed = SPEED;
	float MouseSens;
	float Zoom;
	float Fov = FOV;
	float minView = MIN_VIEW;
	float maxView = MAX_VIEW;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH);

	void setPerspective(float perspective);

	float getPerspective();

	glm::mat4 getProjection();

	glm::mat4 GetViewMatrix();

	void ProcessInputs(Camera_Movement direction, float deltaTime);

	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	void resetRotation();

private:
	void updateCameraVectors();
	float camPerspective;
};

#endif // !CAMERA_H
