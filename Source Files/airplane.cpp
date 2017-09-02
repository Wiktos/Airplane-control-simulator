#include "airplane.h"
#include <iostream>
using namespace std;

void Airplane::rollLeft(float deltaTime)
{
	rollAngle += movementSensitivity * deltaTime;
}
void Airplane::rollRight(float deltaTime)
{
	rollAngle -= movementSensitivity * deltaTime;
}
void Airplane::leanForward(float deltaTime)
{
	pitchAngle += movementSensitivity * deltaTime;
}
void Airplane::leanBackward(float deltaTime)
{
	pitchAngle -= movementSensitivity * deltaTime;
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