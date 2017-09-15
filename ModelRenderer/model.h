#pragma once
#include <vector>

class Model
{
public:
	Model();
	~Model();

	void Initialize(std::string model, std::vector<std::string> textures);
	void Render();
	void Unload();

private:
	//std::vector<Vertex> vertices;
	//Shader* shader;
};