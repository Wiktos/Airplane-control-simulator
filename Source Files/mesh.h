#pragma once
#include <vector>

#include "shprogram.h"
#include "vertex.h"
#include "texture.h"

class Mesh {
	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(ShaderProgram shader);
private:
	void setupMesh();
};