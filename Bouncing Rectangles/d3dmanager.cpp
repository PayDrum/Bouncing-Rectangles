#include "d3dmanager.hpp"



D3dManager::D3dManager()
{
}


HRESULT D3dManager::init(HWND hWnd)
{
	HRESULT hr = S_OK;
	//Create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd = {};

	//Fill the swap chain struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = Settings::getInstance().screenWidth;           // set the back buffer width
	scd.BufferDesc.Height = Settings::getInstance().screenHeight;         // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = hWnd;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	//Create a device, device context and swap chain using the information in the scd struct
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&device,
		NULL,
		&deviceContext);

	if (FAILED(hr))
		return hr;

	//Get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	//Use the back buffer address to create the render target
	hr = device->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	if (FAILED(hr))
		return hr;
	pBackBuffer->Release();

	//Set the render target as the back buffer
	deviceContext->OMSetRenderTargets(1, &backBuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Settings::getInstance().screenWidth;
	viewport.Height = Settings::getInstance().screenHeight;

	deviceContext->RSSetViewports(1, &viewport);

	return hr;
}



D3dManager::~D3dManager()
{
	//cleanup
	swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode
	swapchain->Release();
	backBuffer->Release();
	device->Release();
	deviceContext->Release();
}
