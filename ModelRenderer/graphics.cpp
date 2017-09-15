#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::Initialize(HWND window, SHADER_MODEL MODEL)
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

	InitializeShader(MODEL);

	model = new Model();

	model->Initialize(device, context, "");
}

void Graphics::InitializeShader(SHADER_MODEL MODEL)
{
	//switch (MODEL)
	//{
		//case SHADER_MODEL::AMBIENT_DIFFUSE_SPECULAR:

			ID3DBlob* VS;
			ID3DBlob* PS;

			D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VertexShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
			D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PixelShader",  "ps_4_0", 0, 0, 0, &PS, 0, 0);

			device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
			device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

			context->VSSetShader(pVS, 0, 0);
			context->PSSetShader(pPS, 0, 0);

			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
			context->IASetInputLayout(pLayout);

			//break;

		//case SHADER_MODEL::PHYSICALLY_BASED_RENDERING:

			//break;

		//default:

			//break;
	//}
}

void Graphics::Render()
{
	// clear
	float color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// render
	model->Render();

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