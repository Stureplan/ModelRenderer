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

	MESHINFO LoadModel(std::string filename);
	void Unload();

private:
	MESHINFO BuildMesh(aiMesh* mesh);

private:
	const aiScene* scene;
};

#endif