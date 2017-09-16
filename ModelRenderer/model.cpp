#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, std::string model/*, std::vector<std::string> textures*/)
{
	modelContext = context;

	vertices =
	{
		{ -0.5f, -0.5f, 0.0f},
		{ -0.5f, 0.5f, 0.0f},
		{ 0.5f, 0.5f, 0.0f},
		{ 0.5f, -0.5f, 0.0f}
	};

	indices =
	{
		0, 1, 2,
		0, 2, 3
	};


	// init & create index buffer
	D3D11_BUFFER_DESC bd = { 0 };
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * 2 * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	// init & create vertex buffer
	bd = { 0 };
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX) * vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	device->CreateBuffer(&bd, NULL, &modelVertexBuffer);


	//D3D11_MAPPED_SUBRESOURCE vertexsub;
	//context->Map(modelVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &vertexsub);
	//memcpy(vertexsub.pData, vertices.data(), sizeof(vertices));
	//context->Unmap(modelVertexBuffer, NULL);
	


	D3D11_SUBRESOURCE_DATA indexsub;
	indexsub.pSysMem = indices.data();
	device->CreateBuffer(&bd, &indexsub, &modelIndexBuffer);
	context->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_SUBRESOURCE_DATA vertexsub;
	ZeroMemory(&vertexsub, sizeof(vertexsub));
	vertexsub.pSysMem = vertices.data();
	device->CreateBuffer(&bd, &vertexsub, &modelVertexBuffer);
}

void Model::Render()
{
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);
	modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	modelContext->DrawIndexed(indices.size(), 0, 0);
}

void Model::Unload()
{
	modelContext->Release();
	modelIndexBuffer->Release();
	modelVertexBuffer->Release();
}