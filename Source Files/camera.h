#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	float movementSensitivity;
public:
	Camera(glm::vec3 position) : position(position) 
	{
		front = glm::vec3(0.0f, 0.0f, -1.0f);
		up = glm::vec3(0.0f, 1.0f, 0.0f);
		movementSensitivity = 2.5f;
		updateCameraVectors();
	}

	glm::mat4 viewMatrix()
	{
		return glm::lookAt(position, position + front, up);
	}
	void moveForward(float deltaTime);
	void moveBackward(float deltaTime);
	void moveLeft(float deltaTime);
	void moveRight(float deltaTime);
private:
	void updateCameraVectors()
	{
		right = glm::normalize(glm::cross(front, up));  
		up = glm::normalize(glm::cross(right, front));
	}
};