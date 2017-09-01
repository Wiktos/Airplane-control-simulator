#pragma once
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "model.h"

class Airplane : public Model
{
	glm::mat4 model;
	float rollAngle;
	float pitchAngle;
	float movementSensitivity;
public:
	Airplane(std::string path, float sensitivity) : Model(path), movementSensitivity(sensitivity)
	{
		rollAngle = 0.0f;
		pitchAngle = 0.0f;
	}
	glm::mat4 modelMatrix()
	{
		updateModelMatrix();
		return model;
	}
	void rollLeft(float deltaTime);
	void rollRight(float deltaTime);
	void leanForward(float deltaTime);
	void leanBackward(float deltaTime);
private:
	void updateModelMatrix();
};