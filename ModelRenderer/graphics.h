#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>

#include "structures.h"
#include "model.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")



class Graphics
{
public:
	Graphics();
	~Graphics();

	void Initialize(HWND window, SHADER_MODEL MODEL);
	void InitializeShader(SHADER_MODEL MODEL);
	void Render();
	void Unload();

private:
	
	Model model;

	// gfx pointers
	IDXGISwapChain*			swapchain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backbuffer;

	// shader stuff
	ID3D11InputLayout*	pLayout;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader*	pPS;
};



