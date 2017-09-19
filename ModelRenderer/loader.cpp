#include "loader.h"

Loader::Loader()
{

}
Loader::~Loader()
{

}

MESH Loader::Load(std::string path)
{
	char p[MAX_PATH];
	GetModuleFileNameA(NULL, p, MAX_PATH + 1);

	MessageBoxA(NULL, p, "Error", MB_OK);
	
	path.insert(0, "Data/");
	MESH mesh;

	const aiScene* scene = aiImportFile("C:\\Users\\linus.axelsson3\\Documents\\GitHub\\ModelRenderer\\x64\\Debug\\Data\\export.fbx", aiProcess_ConvertToLeftHanded);

	aiNode* obj = scene->mRootNode->mChildren[0];

	if (scene == NULL)
	{
		//return NULL;
	}

	for (int i = 0; i < obj->mNumMeshes; i++)
	{
		aiMesh* m = scene->mMeshes[i];

		meshes.push_back(m);
		mesh = BuildMesh(meshes[i]);
	}

	return mesh;
}

MESH Loader::BuildMesh(aiMesh* mesh)
{
	MESH m;

	std::vector<VERTEX> vertices;
	std::vector<unsigned int> indices;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vtx;
		aiVector3D vec = mesh->mVertices[i];
		vtx.X = vec.x;
		vtx.Y = vec.y;
		vtx.Z = vec.z;

		vtx.U = mesh->mTextureCoords[0][i].x;
		vtx.V = mesh->mTextureCoords[0][i].y;

		m.vertices.push_back(vtx);
	}

	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; j++)
		{
			m.indices.push_back(face.mIndices[j]);
		}
	}

	return m;
}

void Loader::Unload()
{

}