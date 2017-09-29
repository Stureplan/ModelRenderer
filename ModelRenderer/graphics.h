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
#include "light.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

using namespace DirectX;


#define FORWARD XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f)



class Graphics
{
public:
	Graphics();
	~Graphics();

	void Initialize(HWND window, std::string programpath, std::string meshpath, SHADER_MODEL MODEL);
	void InitializeShader(SHADER_MODEL MODEL);
	void Render(double dT, bool debug);
	void Unload();

	void ParseMessage(std::string msg);

	struct CBUFFER_WVP
	{
		DirectX::XMMATRIX wvp;
		DirectX::XMMATRIX world;
	};

	struct CBUFFER_LIGHTS
	{
		LIGHT lights[4];
		unsigned int lightnumber;
	};

private:
	
	Model model;
	Model box;
	Light lights[1];
	
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

	CBUFFER_WVP cBuffer_matrices;
	ID3D11Buffer* constantbuffer_matrices;

	CBUFFER_LIGHTS cBuffer_lights;
	ID3D11Buffer* constantbuffer_lights;

	// sampler for texteures
	ID3D11SamplerState* textureSamplerState;

	// shader stuff
	ID3D11InputLayout*	pLayout;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader*	pPS;

	ID3D10Blob* VS;
	ID3D10Blob* PS;

	HWND win;
};



