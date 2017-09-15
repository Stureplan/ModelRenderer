#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, std::string model/*, std::vector<std::string> textures*/)
{
	vertices =
	{
		{0.0f, 0.5f, 0.0f},
		{0.45f, -0.5f, 0.0f},
		{-0.45f, -0.5f, 0.0f}
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&bd, NULL, &pVBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices.data(), sizeof(vertices));
	context->Unmap(pVBuffer, NULL);
}

void Model::Render()
{
	
}

void Model::Unload()
{

}