#pragma once
#ifndef LOADER_H
#define LOADER_H

#ifdef _DEBUG
#pragma comment (lib, "assimp-vc140-mtd.lib")
#else
#pragma comment (lib, "assimp-vc140-mt.lib")
#endif


#include <vector>
#include <string>
#include <DirectXMath.h>

#include <..\\AssImp\Importer.hpp>
#include <..\\AssImp\scene.h>
#include <..\\AssImp\postprocess.h>

#include "structures.h"


class Loader
{
public:
	Loader();
	~Loader();

	MESH Load(std::string path);
	void Unload();

private:
	MESH BuildMesh(aiMesh* mesh);

private:
	std::vector<aiMesh*> meshes;
};

#endif