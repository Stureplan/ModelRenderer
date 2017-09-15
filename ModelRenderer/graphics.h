#pragma once
#include <d3d11.h>
#include <d3dx11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")

class Graphics
{
public:
	Graphics();
	~Graphics();

	void Initialize(HWND window);
	void Render();
	void Unload();

private:
	//Model* model;

	IDXGISwapChain*			swapchain;
	ID3D11Device*			device;
	ID3D11DeviceContext*	context;
	ID3D11RenderTargetView* backbuffer;
};