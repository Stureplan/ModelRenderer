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
	FLOAT X, Y, Z;
};