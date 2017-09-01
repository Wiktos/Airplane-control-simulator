#include "camera.h"

void Camera::moveForward(float deltaTime)
{
	float velocity = movementSensitivity * deltaTime;
	position += front * deltaTime;
}

void Camera::moveBackward(float deltaTime)
{
	float velocity = movementSensitivity * deltaTime;
	position -= front * deltaTime;
}

void Camera::moveLeft(float deltaTime)
{
	float velocity = movementSensitivity * deltaTime;
	position -= right * deltaTime;
}

void Camera::moveRight(float deltaTime)
{
	float velocity = movementSensitivity * deltaTime;
	position += right * deltaTime;
}