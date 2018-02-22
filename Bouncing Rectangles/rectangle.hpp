#pragma once

#include <random>
#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>

#include "settings.hpp"
#include "shader.hpp"

class Rect
{
private:

	const int minwidth = 16;
	const int maxWidth = 48;
	const int minHeight = 24;
	const int maxHeight = 64;
	const int minSpeed = 10;
	const int maxSpeed = 100;

	const int screenHeight, screenWidth;

	float width;
	float height;
	float red, green, blue;

	int speed;
	float vx, vy;
	float direction;

	float currentX;
	float currentY;


	void generateRandomArguments();
	void createGeometry();

public:

	ID3D11Buffer* cbo = nullptr;		//The pointer to the constant buffer object
	ID3D11Buffer* ibo = nullptr;		//The pointer to the index buffer object
	ID3D11Buffer* vbo = nullptr;        //The pointer to the vertex buffer object
	ShaderObj shaders;

	struct vertex { FLOAT X, Y, Z; FLOAT R, G, B, A; };
	vertex vertices[4];
	unsigned int indices[6] = 
	{ 
		0,1,2,
		0,1,3 
	};
	DirectX::XMMATRIX modelMat;

	void updateCurrentPosition(FLOAT delta);
	void updateModelMat();

	Rect();
	~Rect();

};