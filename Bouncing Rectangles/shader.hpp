#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <D3Dcompiler.h>



class ShaderObj
{

public:

	ID3D11InputLayout* layout = nullptr;					 // the pointer to the input layout
	ID3D11VertexShader* vertexShader = nullptr;            // the pointer to the vertex shader
	ID3D11PixelShader* pixelShader = nullptr;              // the pointer to the pixel shader

	HRESULT load(ID3D11Device*, ID3D11DeviceContext*);
};