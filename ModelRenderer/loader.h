#pragma once
#ifndef LOADER_H
#define LOADER_H

#ifdef _DEBUG
#pragma comment (lib, "assimp-vc140-mtd.lib")
#else
#pragma comment (lib, "assimp-vc140-mt.lib")
#endif

#include <assimp\cimport.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <Windows.h>
#include <Shlwapi.h>

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "structures.h"


class Loader
{
public:
	Loader();
	~Loader();

	MESH Load(std::string filename);
	void Unload();

private:
	MESH BuildMesh(aiMesh* mesh);

private:
	std::vector<aiMesh*> meshes;
	const aiScene* scene;
};

#endif