#pragma once
#include <vector>
#include <d3dx11.h>


#include "structures.h"

class Model
{
public:
	Model();
	~Model();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context, std::string model/*, std::vector<std::string> textures*/);
	void Render();
	void Unload();

private:
	std::vector<VERTEX> vertices;
	std::vector<int>	indices;

	ID3D11Buffer*		modelVertexBuffer;
	ID3D11Buffer*		modelIndexBuffer;
	ID3D11DeviceContext* modelContext;
};