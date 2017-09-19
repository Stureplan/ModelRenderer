#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::Initialize(HWND window,std::string programpath, std::string meshpath, SHADER_MODEL SHADER_MODEL)
{
	win = window;
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = W;
	bufferDesc.Height = H;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapDesc;

	ZeroMemory(&swapDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapDesc.BufferDesc = bufferDesc;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 1;
	swapDesc.OutputWindow = window;
	swapDesc.Windowed = TRUE;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL,
		D3D11_SDK_VERSION,
		&swapDesc,
		&swapchain,
		&device,
		NULL,
		&context
	);

	ID3D11Texture2D* backBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	device->CreateRenderTargetView(backBuffer, NULL, &renderTarget);
	backBuffer->Release();

	D3D11_TEXTURE2D_DESC depthDesc;

	depthDesc.Width = W;
	depthDesc.Height = H;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	

	device->CreateTexture2D(&depthDesc, NULL, &stencilbuffer);
	device->CreateDepthStencilView(stencilbuffer, NULL, &stencil);

	context->OMSetRenderTargets(1, &renderTarget, stencil);

	InitializeShader(SHADER_MODEL);

	model.LoadMesh(device, context, loader.Load(meshpath));

	// viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = W;
	viewport.Height = H;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports(1, &viewport);


	D3D11_BUFFER_DESC cbDesc;
	ZeroMemory(&cbDesc, sizeof(D3D11_BUFFER_DESC));

	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	cbDesc.ByteWidth = sizeof(cBuffer);
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	device->CreateBuffer(&cbDesc, NULL, &constantbuffer);

	camPos		= XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	camTarget	= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	World		= XMMatrixIdentity();
	View		= XMMatrixLookAtLH(camPos, camTarget, camUp);
	Projection	= XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)W / (float)H, 1.0f, 1000.0f);
}

void Graphics::InitializeShader(SHADER_MODEL MODEL)
{
	//switch (MODEL)
	//{
		//case SHADER_MODEL::AMBIENT_DIFFUSE_SPECULAR:

			HRESULT r;
			r = D3DX11CompileFromFileA("C:\\Users\\linus.axelsson3\\Documents\\GitHub\\ModelRenderer\\x64\\Debug\\shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
			if (r != S_OK)
			{
				MessageBoxA(win, "Vertex Shader failed to load!", "Error", MB_OK);
			}
			
			D3DX11CompileFromFileA("C:\\Users\\linus.axelsson3\\Documents\\GitHub\\ModelRenderer\\x64\\Debug\\shaders.shader", 0, 0, "PShader",  "ps_4_0", 0, 0, 0, &PS, 0, 0);
			if (r != S_OK)
			{
				MessageBoxA(win, "Pixel Shader failed to load!", "Error", MB_OK);
			}

			device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
			device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
			
			context->VSSetShader(pVS, 0, 0);
			context->PSSetShader(pPS, 0, 0);
			
			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
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
	context->ClearRenderTargetView(renderTarget, color);
	context->ClearDepthStencilView(stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	World = XMMatrixIdentity();

	WVP = model.Matrix() * View * Projection;

	cBuffer.WVP = XMMatrixTranspose(WVP);

	context->UpdateSubresource(constantbuffer, 0, NULL, &cBuffer, 0, 0);
	context->VSSetConstantBuffers(0, 1, &constantbuffer);
	
	// render
	model.Render();

	// swap
	swapchain->Present(0, 0);
}

void Graphics::Unload()
{
	swapchain		->Release();
	device			->Release();
	context			->Release();
	renderTarget	->Release();
	stencil			->Release();
	stencilbuffer	->Release();
	constantbuffer	->Release();
	pLayout			->Release();
	pVS				->Release();
	pPS				->Release();
}