#pragma once
#include <string>
#include <glm\glm.hpp>
#include <assimp\Importer.hpp>

struct Texture
{
	aiString path;
	unsigned int id;
	std::string type;
};