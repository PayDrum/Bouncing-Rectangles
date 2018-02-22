#include "shader.hpp"


HRESULT ShaderObj::load(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hr = S_OK;
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	hr = D3DCompileFromFile(L"shaders.shader", NULL, NULL, "VShader", "vs_5_0", 0, 0, &VS, 0);
		//0, 0, "VShader", "vs_4_0", 0, &VS, 0, 0);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Unable to compile vertex shader", "Error", MB_OK);
		return hr;
	}

	hr = D3DCompileFromFile(L"shaders.shader", NULL, NULL, "PShader", "ps_5_0", 0, 0, &PS, 0);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			"Unable to compile pixel shader", "Error", MB_OK);
		return hr;
	}

	// encapsulate both shaders into shader objects
	hr = device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &vertexShader);
	if (FAILED(hr))
	{
		VS->Release();
		return hr;
	}
	hr = device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pixelShader);
	if (FAILED(hr))
	{
		PS->Release();
		return hr;
	}
	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &layout);
	if (FAILED(hr))
		return hr;
	deviceContext->IASetInputLayout(layout);

	return hr;
}