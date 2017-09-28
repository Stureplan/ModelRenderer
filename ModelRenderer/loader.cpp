#include "loader.h"

Loader::Loader()
{

}
Loader::~Loader()
{

}

MESHINFO Loader::LoadModel(std::string filename)
{
	//	\\Data\\abc.fbx
	filename.insert(0, "Data\\");

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


	MESHINFO mesh;
	TEXTUREINFO textures;
	textures.HAS_TEXTURE = false;
	textures.HAS_NORMALMAP = false;
	textures.HAS_SPECULARMAP = false;
	
	scene = aiImportFile(fullpath.c_str(), aiProcess_ConvertToLeftHanded);
	if (scene == NULL)
	{
		return MESHINFO::EMPTYMESH();
	}

	aiNode* obj = scene->mRootNode->mChildren[0];
	if (scene->HasMaterials())
	{
		aiMaterial* mat = scene->mMaterials[0];
		aiString path;

		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			textures.HAS_TEXTURE = true;
			textures.TEXTURENAMES.push_back(PathFindFileNameA(path.C_Str()));
		}

		if (mat->GetTexture(aiTextureType_NORMALS, 1, &path) == AI_SUCCESS)
		{
			textures.HAS_NORMALMAP = true;
			textures.TEXTURENAMES.push_back(PathFindFileNameA(path.C_Str()));
		}
		
		if (mat->GetTexture(aiTextureType_SPECULAR, 2, &path) == AI_SUCCESS)
		{
			textures.HAS_SPECULARMAP = true;
			textures.TEXTURENAMES.push_back(PathFindFileNameA(path.C_Str()));
		}
	}

	mesh = BuildMesh(scene->mMeshes[0]);
	mesh.textureinfo = textures;

	return mesh;
}

MESHINFO Loader::BuildMesh(aiMesh* mesh)
{
	MESHINFO m;
	std::vector<VERTEX> vertices;
	std::vector<unsigned int> indices;

	bool hasUVS = mesh->HasTextureCoords(0);
	bool hasTBN = mesh->HasTangentsAndBitangents() && mesh->HasNormals();

	if (hasUVS == false || hasTBN == false)
	{
		return MESHINFO::EMPTYMESH();
	}

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vtx;


		aiVector3D vec = mesh->mVertices[i];
		vtx.X = vec.x;
		vtx.Y = vec.y;
		vtx.Z = vec.z;

		if (hasUVS)
		{
			aiVector3D uvs = mesh->mTextureCoords[0][i];
			vtx.U = uvs.x;
			vtx.V = uvs.y;
		}


		if (hasTBN)
		{
			aiVector3D tan = mesh->mTangents[i];
			aiVector3D bit = mesh->mBitangents[i];
			aiVector3D nor = mesh->mNormals[i];
			vtx.T_X = tan.x;
			vtx.T_Y = tan.z;
			vtx.T_Z = tan.y;
			vtx.B_X = bit.x;
			vtx.B_Y = bit.z;
			vtx.B_Z = bit.y;
			vtx.N_X = nor.x;
			vtx.N_Y = nor.z;
			vtx.N_Z = nor.y;
		}

		m.vertices.push_back(vtx);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m.indices.push_back(face.mIndices[j]);
		}
	}

	return m;
}

void Loader::Unload()
{
	scene->~aiScene();
}