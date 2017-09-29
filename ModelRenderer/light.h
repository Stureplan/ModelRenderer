#pragma once
#include <d3dx11.h>
#include <DirectXMath.h>
#include "structures.h"


/*
#define COLOR_RED	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
*/
using namespace DirectX;

class Light
{
public:
	Light();
	~Light();
	
	LIGHT GetLight();
	void Create(XMFLOAT4 position, XMFLOAT4 color, float intensity);
	void Move(XMFLOAT4 position);

	XMMATRIX Matrix();

private:
	LIGHT light;
};