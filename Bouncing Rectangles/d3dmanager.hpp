#pragma once
#include <d3d11.h>

#include "settings.hpp"

class D3dManager
{
private:

	 IDXGISwapChain*  swapchain;             // the pointer to the swap chain interface
	 ID3D11Device*  device;                  // the pointer to our Direct3D device interface
	 ID3D11DeviceContext*  deviceContext;    // the pointer to our Direct3D device context
	 ID3D11RenderTargetView*   backBuffer;    // the pointer to our back buffer

public:

	D3dManager();
	~D3dManager();

	HRESULT init(HWND hWnd);

	//Get methods
	IDXGISwapChain* getSwapchain() { return this->swapchain; }
	ID3D11Device* getDevice() { return this->device; }
	ID3D11DeviceContext* getDeviceContext() { return this->deviceContext; }
	ID3D11RenderTargetView* getBackBuffer(){ return this->backBuffer;}

};