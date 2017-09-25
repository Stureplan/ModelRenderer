#include "loader.h"

Loader::Loader()
{

}
Loader::~Loader()
{

}

MESH Loader::Load(std::string filename)
{
	filename.insert(0, "Data\\");	//		\\Data\\abc.fbx


	// Full .exe path of program
	char p[MAX_PATH];
	GetModuleFileNameA(NULL, p, MAX_PATH + 1);
	std::string fullpath = p;

	// Only the .exe name (ModelRenderer.exe)
	std::string f;
	f = PathFindFileNameA(fullpath.c_str());

	// Remove ModelRenderer.exe from the full path
	fullpath.erase(fullpath.size() - f.size(), f.size());

	// Insert the shader path
	fullpath.insert(fullpath.size(), filename);

	MESH mesh;
	
	const aiScene* scene = aiImportFile(fullpath.c_str(), aiProcess_ConvertToLeftHanded);

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

	scene->~aiScene();

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
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->~aiMesh();
	}
}