#include "airplane.h"
#include <iostream>
using namespace std;

void Airplane::rollLeft(float deltaTime)
{
	rollAngle += movementSensitivity * deltaTime;
	if (rollAngle > 45.0f)
		rollAngle = 45.0f;
}
void Airplane::rollRight(float deltaTime)
{
	rollAngle -= movementSensitivity * deltaTime;
	if (rollAngle < -45.0f)
		rollAngle = -45.0f;
}
void Airplane::leanForward(float deltaTime)
{
	pitchAngle += movementSensitivity * deltaTime;
	if (pitchAngle > 20.0f)
		pitchAngle = 20.0f;
}
void Airplane::leanBackward(float deltaTime)
{
	pitchAngle -= movementSensitivity * deltaTime;
	if (pitchAngle < -20.0f)
		pitchAngle = -20.0f;
}
void Airplane::updateModelMatrix()
{
	model = glm::mat4();
	model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rollAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(pitchAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-10.0f, -30.0f, 0.0f));
}