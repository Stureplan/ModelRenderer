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

	void Mesh(ID3D11Device* device, ID3D11DeviceContext* context, MESHINFO m);
	void Box(ID3D11Device* device, ID3D11DeviceContext* context, MESHINFO b);
	MESHINFO GetMesh();
	void Render();
	void Unload();

	XMMATRIX Matrix();

private:
	void Textures(ID3D11Device* device, ID3D11DeviceContext* context, TEXTUREINFO t);

	ID3D11Buffer*		modelVertexBuffer;
	ID3D11Buffer*		modelIndexBuffer;
	ID3D11DeviceContext* modelContext;	

	ID3D11RasterizerState* wireFrameState;
	bool wireframe;

	std::vector<ID3D11ShaderResourceView*> textures;

	XMMATRIX World;
	XMMATRIX Rotation;
	XMMATRIX Translation;

	MESHINFO mesh;
	UINT stride;
	UINT offset;

	float rot = 0.0f;
};