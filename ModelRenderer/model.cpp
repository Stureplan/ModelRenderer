#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

void Model::LoadMesh(ID3D11Device* device, ID3D11DeviceContext* context, MESH m)
{
	modelContext = context;

	vertices = m.vertices;
	indices = m.indices;

	D3D11_BUFFER_DESC vDesc = { 0 };
	vDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vDesc.ByteWidth = sizeof(VERTEX) * vertices.size();
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;
	vDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vSub;
	ZeroMemory(&vSub, sizeof(vSub));
	vSub.pSysMem = vertices.data();
	device->CreateBuffer(&vDesc, &vSub, &modelVertexBuffer);


	D3D11_BUFFER_DESC iDesc = { 0 };
	iDesc.Usage = D3D11_USAGE_DEFAULT;
	iDesc.ByteWidth = sizeof(unsigned int) * indices.size();
	iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iDesc.CPUAccessFlags = 0;
	iDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iSub;
	ZeroMemory(&iSub, sizeof(iSub));
	iSub.pSysMem = indices.data();
	device->CreateBuffer(&iDesc, &iSub, &modelIndexBuffer);
	context->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);
	modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::InitializeCube(ID3D11Device* device, ID3D11DeviceContext* context, std::string model/*, std::vector<std::string> textures*/)
{
	modelContext = context;

	vertices =
	{
		{-1.0f, -1.0f, -1.0f},
		{-1.0f, +1.0f, -1.0f},
		{+1.0f, +1.0f, -1.0f},
		{+1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f, +1.0f},
		{-1.0f, +1.0f, +1.0f},
		{+1.0f, +1.0f, +1.0f},
		{+1.0f, -1.0f, +1.0f}
	};

	indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};


	// init & create index buffer
	D3D11_BUFFER_DESC bd = { 0 };
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(unsigned int) * 12 * 3;
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
	

	D3D11_SUBRESOURCE_DATA indexsub;
	indexsub.pSysMem = indices.data();
	device->CreateBuffer(&bd, &indexsub, &modelIndexBuffer);
	context->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3D11_SUBRESOURCE_DATA vertexsub;
	ZeroMemory(&vertexsub, sizeof(vertexsub));
	vertexsub.pSysMem = vertices.data();
	device->CreateBuffer(&bd, &vertexsub, &modelVertexBuffer);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);
	modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Render()
{
	modelContext->DrawIndexed(indices.size(), 0, 0);
}

void Model::Unload()
{
	modelContext->Release();
	modelIndexBuffer->Release();
	modelVertexBuffer->Release();
}

XMMATRIX Model::Matrix()
{
	rot += 0.0001f;
	if (rot > 6.26f)
	{
		rot = 0.0f;
	}


	XMVECTOR rotVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Rotation = XMMatrixRotationAxis(rotVector, rot);
	Translation = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	World = XMMatrixIdentity();
	World = World * Rotation * Translation;
	return World;
}