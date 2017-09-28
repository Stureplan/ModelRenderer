#pragma once

#define W 800
#define H 600


enum SHADER_MODEL
{
	AMBIENT_DIFFUSE_SPECULAR,
	PHYSICALLY_BASED_RENDERING
};

struct VERTEX
{
	float X, Y, Z;
	float U, V;

	float T_X, T_Y, T_Z;
	float B_X, B_Y, B_Z;
	float N_X, N_Y, N_Z;
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
		MESHINFO m;
		m.vertices.clear();
		m.indices.clear();

		return m;
	}
};

struct NET_MESSAGE
{
	float x;
	float y;
	float z;
};