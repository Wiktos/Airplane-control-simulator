#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <SOIL.h>
#include <GL\glew.h>
#include <glm\glm.hpp>

class Skybox
{
	unsigned int cubemapTexture;
	std::vector<std::string> faces
	{
		"./Resource Files/textures/cubemap/right.jpg",
		"./Resource Files/textures/cubemap/left.jpg",
		"./Resource Files/textures/cubemap/top.jpg",
		"./Resource Files/textures/cubemap/bottom.jpg",
		"./Resource Files/textures/cubemap/back.jpg",
		"./Resource Files/textures/cubemap/front.jpg"
	};
	unsigned int VAO;
	unsigned int VBO;
	std::vector<float> skybox_coords;
public:
	Skybox();
	void draw();
private:
	void setCoords();
	void genArray();
	void loadCubemap();
};