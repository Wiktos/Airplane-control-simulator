#pragma once
#include <vector>
#include <string>

#include <assimp\Importer.hpp>
#include <assimp\postprocess.h>
#include <assimp\scene.h>

#include <GL\glew.h>

#include "texture.h"
#include "mesh.h"

class Model
{
	std::vector<Texture> textures_loaded;	
	std::vector<Mesh> meshes;
	std::string directory;
public:
	Model(std::string const &path)
	{
		loadModel(path);
	}
	void draw(ShaderProgram shader);
private:
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Vertex> getMeshVertices(aiMesh* mesh);
	std::vector<unsigned int> getMeshIndices(aiMesh* mesh);
	std::vector<Texture> getMeshTextures(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	unsigned int textureFromFile(const char *path, const std::string &directory);
};