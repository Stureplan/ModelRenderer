#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::Initialize(HWND window)
{
	// initialize DX11
	DXGI_SWAP_CHAIN_DESC desc;

	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferCount = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = window;
	desc.SampleDesc.Count = 4;
	desc.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&desc,
		&swapchain,
		&device,
		NULL,
		&context
	);

	// render target
	ID3D11Texture2D* backBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	device->CreateRenderTargetView(backBuffer, NULL, &backbuffer);
	backBuffer->Release();
	context->OMSetRenderTargets(1, &backbuffer, NULL);

	// viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 1024;
	viewport.Height = 768;

	context->RSSetViewports(1, &viewport);
}

void Graphics::Render()
{
	// clear
	float color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// render
	//model->Render();

	// swap
	swapchain->Present(0, 0);
}

void Graphics::Unload()
{
	swapchain	->Release();
	backbuffer	->Release();
	device		->Release();
	context		->Release();
}