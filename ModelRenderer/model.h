#pragma once
#include <vector>
#include <d3dx11.h>
#include <DirectXMath.h>

#include "structures.h"

using namespace DirectX;

class Model
{
public:
	Model();
	~Model();

	void LoadMesh(ID3D11Device* device, ID3D11DeviceContext* context, MESH m);
	void LoadBox(ID3D11Device* device, ID3D11DeviceContext* context, MESH b);
	MESH GetMesh();
	void Render();
	void Unload();

	XMMATRIX Matrix();

private:

	ID3D11Buffer*		modelVertexBuffer;
	ID3D11Buffer*		modelIndexBuffer;
	ID3D11DeviceContext* modelContext;	

	ID3D11RasterizerState* wireFrameState;
	bool wireframe;

	XMMATRIX World;
	XMMATRIX Rotation;
	XMMATRIX Translation;

	MESH mesh;
	UINT stride;
	UINT offset;

	float rot = 0.0f;
};