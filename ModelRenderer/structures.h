#pragma once

#define W 800
#define H 600

#include <vector>

enum SHADER_MODEL
{
	AMBIENT_DIFFUSE_SPECULAR,
	PHYSICALLY_BASED_RENDERING
};

enum LIGHT_TYPE
{
	DIRECTIONAL_LIGHT,
	POINT_LIGHT
};

struct VERTEX
{
	float X, Y, Z;
	float U, V;

	float T_X, T_Y, T_Z;
	float B_X, B_Y, B_Z;
	float N_X, N_Y, N_Z;
};

struct LIGHT
{
	float POS_X, POS_Y, POS_Z, POS_W;
	float COL_R, COL_G, COL_B, COL_A;
	float INTENSITY;
};

struct TEXTUREINFO
{
	bool HAS_TEXTURE;
	bool HAS_NORMALMAP;
	bool HAS_SPECULARMAP;

	std::vector<std::string> TEXTURENAMES;
};

struct MESHINFO
{
	std::vector<VERTEX> vertices;
	std::vector<unsigned int> indices;

	TEXTUREINFO textureinfo;

	static MESHINFO EMPTYMESH()
	{
		MESHINFO M;
		M.vertices.clear();
		M.indices.clear();

		return M;
	}
};

struct NET_MESSAGE
{
	float x;
	float y;
	float z;
};