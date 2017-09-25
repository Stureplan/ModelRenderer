#include "model.h"

Model::Model()
{

}

Model::~Model()
{

}

MESH Model::GetMesh()
{
	return mesh;
}

void Model::LoadMesh(ID3D11Device* device, ID3D11DeviceContext* context, MESH m)
{
	wireframe = false;
	modelContext = context;

	mesh = m;

	D3D11_BUFFER_DESC vDesc = { 0 };
	vDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vDesc.ByteWidth = sizeof(VERTEX) * mesh.vertices.size();
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;
	vDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vSub;
	ZeroMemory(&vSub, sizeof(vSub));
	vSub.pSysMem = mesh.vertices.data();
	device->CreateBuffer(&vDesc, &vSub, &modelVertexBuffer);


	D3D11_BUFFER_DESC iDesc = { 0 };
	iDesc.Usage = D3D11_USAGE_DEFAULT;
	iDesc.ByteWidth = sizeof(unsigned int) * mesh.indices.size();
	iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iDesc.CPUAccessFlags = 0;
	iDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iSub;
	ZeroMemory(&iSub, sizeof(iSub));
	iSub.pSysMem = mesh.indices.data();
	device->CreateBuffer(&iDesc, &iSub, &modelIndexBuffer);
	context->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	stride = sizeof(VERTEX);
	offset = 0;

	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);
	modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Model::LoadBox(ID3D11Device* device, ID3D11DeviceContext* context, MESH b)
{
	wireframe = true;
	modelContext = context;

	/*aiVector3D min = aiVector3D(0.0f, 0.0f, 0.0f);
	aiVector3D max = aiVector3D(0.0f, 0.0f, 0.0f);
	// should be bounding box --v
	if (vtx.X < min.x) min.x = vtx.X;
	if (vtx.Y < min.y) min.y = vtx.Y;
	if (vtx.Z < min.z) min.z = vtx.Z;

	if (vtx.X > max.x) max.x = vtx.X;
	if (vtx.Y > max.y) max.y = vtx.Y;
	if (vtx.Z > max.z) max.z = vtx.Z;*/


	VERTEX min = { 0.0f, 0.0f, 0.0f };
	VERTEX max = { 0.0f, 0.0f, 0.0f };

	// find min & max
	for (int i = 0; i < b.vertices.size(); i++)
	{
		VERTEX v = b.vertices[i];

		if (v.X < min.X) min.X = v.X;
		if (v.Y < min.Y) min.Y = v.Y;
		if (v.Z < min.Z) min.Z = v.Z;

		if (v.X > max.X) max.X = v.X;
		if (v.Y > max.Y) max.Y = v.Y;
		if (v.Z > max.Z) max.Z = v.Z;
	}

	mesh.vertices =
	{
		{ min.X, min.Y, min.Z, 1.0f, 1.0f },
		{ min.X, max.Y, min.Z, 1.0f, 1.0f },
		{ max.X, max.Y, min.Z, 1.0f, 1.0f },
		{ max.X, min.Y, min.Z, 1.0f, 1.0f },
		{ min.X, min.Y, max.Z, 1.0f, 1.0f },
		{ min.X, max.Y, max.Z, 1.0f, 1.0f },
		{ max.X, max.Y, max.Z, 1.0f, 1.0f },
		{ max.X, min.Y, max.Z, 1.0f, 1.0f }
	};


	mesh.indices =
	{
		// -z face
		0, 1, 2, 3, 
		
		// intermediary
		0,

		// -x face
		4, 5, 1, 0,

		// -y face
		4, 7, 3, 0,

		// intermediary
		4,

		// +z face
		5, 6, 7, 4,

		// +x face
		7, 6, 2, 3, 

		// intermediary
		2,

		// +y face
		1, 5, 6, 2
	};


	/*mesh.indices =
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
	};*/

	D3D11_BUFFER_DESC vDesc = { 0 };
	vDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vDesc.ByteWidth = sizeof(VERTEX) * mesh.vertices.size();
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;
	vDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vSub;
	ZeroMemory(&vSub, sizeof(vSub));
	vSub.pSysMem = mesh.vertices.data();
	device->CreateBuffer(&vDesc, &vSub, &modelVertexBuffer);


	D3D11_BUFFER_DESC iDesc = { 0 };
	iDesc.Usage = D3D11_USAGE_DEFAULT;
	iDesc.ByteWidth = sizeof(unsigned int) * mesh.indices.size();
	iDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iDesc.CPUAccessFlags = 0;
	iDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iSub;
	ZeroMemory(&iSub, sizeof(iSub));
	iSub.pSysMem = mesh.indices.data();
	device->CreateBuffer(&iDesc, &iSub, &modelIndexBuffer);
	modelContext->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	stride = sizeof(VERTEX);
	offset = 0;

	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);
	modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);


	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&rd, &wireFrameState);
}

void Model::Render()
{
	if (wireframe) 
	{ 
		modelContext->RSSetState(wireFrameState); 
		modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}
	else
	{ 
		modelContext->RSSetState(NULL); 
		modelContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}


	modelContext->IASetIndexBuffer(modelIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	modelContext->IASetVertexBuffers(0, 1, &modelVertexBuffer, &stride, &offset);

	modelContext->DrawIndexed(mesh.indices.size(), 0, 0);
}

void Model::Unload()
{
	modelContext		->Release();
	modelIndexBuffer	->Release();
	modelVertexBuffer	->Release();
	if (wireframe)wireFrameState		->Release();
}

XMMATRIX Model::Matrix()
{
	rot += 0.0001f;
	if (rot > 6.26f)
	{
		rot = 0.0f;
	}


	XMVECTOR rotVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//Rotation = XMMatrixRotationAxis(rotVector, rot);
	Rotation = XMMatrixIdentity();
	Translation = XMMatrixIdentity();
	World = XMMatrixIdentity();
	World = World * Rotation * Translation;
	return World;
}