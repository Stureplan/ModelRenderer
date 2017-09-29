#include "graphics.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::Initialize(HWND window,std::string programpath, std::string meshpath, SHADER_MODEL SHADER_MODEL)
{
	renderTarget = NULL;

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
	
	// depth stencil view
	device->CreateTexture2D(&depthDesc, NULL, &stencilbuffer);
	device->CreateDepthStencilView(stencilbuffer, NULL, &stencil);
	context->OMSetRenderTargets(1, &renderTarget, stencil);


	// for textures
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerDesc, &textureSamplerState);



	InitializeShader(SHADER_MODEL);

	model.Mesh(device, context, loader.LoadModel(meshpath));
	box.Box(device, context, model.GetMesh());
	
	lights[0].Create(XMFLOAT4(-4.0f, 0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
	//lights[1].Create(XMFLOAT3(-4.0f, 0.0f, 0.0f), COLOR_RED, 1.0f);
	//lights[2].Create(XMFLOAT3(-4.0f, 0.0f, 0.0f), COLOR_RED, 1.0f);
	//lights[3].Create(XMFLOAT3(-4.0f, 0.0f, 0.0f), COLOR_RED, 1.0f);


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

	// constant buffer (WVP)
	D3D11_BUFFER_DESC cb_wvp_Desc;
	ZeroMemory(&cb_wvp_Desc, sizeof(D3D11_BUFFER_DESC));
	cb_wvp_Desc.Usage = D3D11_USAGE_DEFAULT;
	cb_wvp_Desc.ByteWidth = sizeof(cBuffer_matrices);
	cb_wvp_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_wvp_Desc.CPUAccessFlags = 0;
	cb_wvp_Desc.MiscFlags = 0;
	cb_wvp_Desc.StructureByteStride = 0;
	device->CreateBuffer(&cb_wvp_Desc, NULL, &constantbuffer_matrices);

	// constant buffer (lights)
	D3D11_BUFFER_DESC cb_lights_Desc;
	ZeroMemory(&cb_lights_Desc, sizeof(D3D11_BUFFER_DESC));
	cb_lights_Desc.Usage = D3D11_USAGE_DEFAULT;
	cb_lights_Desc.ByteWidth = sizeof(cBuffer_lights);
	cb_lights_Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_lights_Desc.CPUAccessFlags = 0;
	cb_lights_Desc.MiscFlags = 0;
	cb_lights_Desc.StructureByteStride = 0;
	device->CreateBuffer(&cb_wvp_Desc, NULL, &constantbuffer_lights);


	camPos		= XMVectorSet(0.0f, 1.0f, -6.0f, 0.0f);
	camTarget	= camPos + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
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

	// Full .exe path of program
	char p[MAX_PATH];
	GetModuleFileNameA(NULL, p, MAX_PATH+1);
	std::string fullpath = p;

	// Only the .exe name (ModelRenderer.exe)
	std::string f;
	f = PathFindFileNameA(fullpath.c_str());

	// Remove ModelRenderer.exe from the full path
	fullpath.erase(fullpath.size() - f.size(), f.size());

	// Insert the shader path
	fullpath.insert(fullpath.size(), "shaders.shader");


	HRESULT r;
	ID3D10Blob* blob;

	blob = NULL;
	r = D3DX11CompileFromFileA(fullpath.c_str(), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &blob, 0);
	if (r != S_OK)
	{
		std::string err = static_cast<char*>(blob->GetBufferPointer());

		MessageBoxA(win, err.c_str(), "Error", MB_OK);
	}
	
	blob = NULL;
	D3DX11CompileFromFileA(fullpath.c_str(), 0, 0, "PShader",  "ps_4_0", 0, 0, 0, &PS, &blob, 0);
	if (r != S_OK)
	{
		std::string err = static_cast<char*>(blob->GetBufferPointer());

		MessageBoxA(win, err.c_str(), "Error", MB_OK);
	}

	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);
			
	context->VSSetShader(pVS, 0, 0);
	context->PSSetShader(pPS, 0, 0);
			
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",	   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr=	device->CreateInputLayout(ied, sizeof(ied)/sizeof(ied[0]), VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	context->IASetInputLayout(pLayout);
			//break;

		//case SHADER_MODEL::PHYSICALLY_BASED_RENDERING:

			//break;

		//default:

			//break;
	//}
}

void Graphics::Render(double dT, bool debug)
{
	// clear
	float color[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	context->ClearRenderTargetView(renderTarget, color);
	context->ClearDepthStencilView(stencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// prep render
	World = XMMatrixIdentity();
	View  = XMMatrixLookAtLH(camPos, camTarget, camUp);
	WVP   = model.Matrix() * View * Projection;

	// cbuffer wvp
	cBuffer_matrices.wvp   = XMMatrixTranspose(WVP);
	cBuffer_matrices.world = XMMatrixTranspose(World);
	context->UpdateSubresource(constantbuffer_matrices, 0, NULL, &cBuffer_matrices, 0, 0);
	context->VSSetConstantBuffers(0, 1, &constantbuffer_matrices);

	// cbuffer lights
	unsigned int lightnumber = sizeof(lights) / sizeof(lights[0]);
	for (unsigned int i = 0; i < lightnumber; i++)
	{
		cBuffer_lights.lights[i] = lights[i].GetLight();
		cBuffer_lights.lightnumber = lightnumber;
	}
	context->UpdateSubresource(constantbuffer_lights, 0, NULL, &cBuffer_lights, 0, 0);
	context->PSSetConstantBuffers(0, 1, &constantbuffer_lights);

	context->PSSetSamplers(0, 1, &textureSamplerState);

	// render
	model.Render();

	/*if (debug)
	{
		// TODO: fix constant buffer updating when debug is true.
		// right now the matrices are fucked

		// prep render
		World = XMMatrixIdentity();
		View = XMMatrixLookAtLH(camPos, camTarget, camUp);
		WVP = box.Matrix() * View * Projection;
		cBuffer_matrices.wvp = XMMatrixTranspose(WVP);

		context->UpdateSubresource(constantbuffer_matrices, 0, NULL, &constantbuffer_matrices, 0, 0);
		context->VSSetConstantBuffers(0, 1, &constantbuffer_matrices);

		box.Render();
	}*/


	// swap
	swapchain->Present(0, 0);
}

void Graphics::ParseMessage(std::string msg)
{
	NET_MESSAGE* message;

	message = (NET_MESSAGE*)msg.c_str();

	float x = message->x;
	float y = message->y;
	float z = message->z;

	camPos = XMVectorSet(x, y, z, 0.0f);
	camTarget = camPos + XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	std::string pos = std::to_string(z);
	pos.insert(0, "Z: ");
	pos.insert(0, std::to_string(y));
	pos.insert(0, "Y: ");

	pos.insert(0, std::to_string(x));
	pos.insert(0, "X: ");


	SetWindowTextA(win, pos.c_str());
}

void Graphics::Unload()
{
	model.Unload();

	swapchain		->Release();
	device			->Release();
	context			->Release();
	renderTarget	->Release();
	stencil			->Release();
	stencilbuffer	->Release();
	constantbuffer_matrices->Release();
	pLayout			->Release();
	pVS				->Release();
	pPS				->Release();
	VS				->Release();
	PS				->Release();

	loader.Unload();
}