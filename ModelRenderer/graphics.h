#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <DirectXMath.h>

#include <Windows.h>
#include "Shlwapi.h"

#include "loader.h"
#include "structures.h"
#include "model.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

using namespace DirectX;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Initialize(HWND window, std::string programpath, std::string meshpath, SHADER_MODEL MODEL);
	void InitializeShader(SHADER_MODEL MODEL);
	void Render();
	void Unload();


	struct CBUFFER
	{
		DirectX::XMMATRIX WVP;
	};

private:
	
	Model model;
	Loader loader;


	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;

	DirectX::XMVECTOR camPos;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;


private:
	// gfx pointers
	IDXGISwapChain*			swapchain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* renderTarget;
	ID3D11DepthStencilView* stencil;
	ID3D11Texture2D*		stencilbuffer;

	CBUFFER cBuffer;
	ID3D11Buffer* constantbuffer;

	// shader stuff
	ID3D11InputLayout*	pLayout;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader*	pPS;

	ID3D10Blob* VS;
	ID3D10Blob* PS;
};



