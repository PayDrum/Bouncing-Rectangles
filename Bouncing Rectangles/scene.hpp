#pragma once

#include <DirectXMath.h>
#include <memory>

#include "d3dmanager.hpp"
#include "rectangle.hpp"

class Scene 
{
private:


	D3dManager* d3dManager;

	const int minRectanglesNumber = 3;
	const int maxRectanglesNumber = 9;
	int numberOfRectangles;

	std::vector<std::shared_ptr<Rect>> recVec;	//vector for holding instances of the rectangles

public:

	HRESULT initGraphics();	
	HRESULT initShaders();

	void update(FLOAT);							//Method for updating the scene
	void render();

	Scene(D3dManager*);							//An instace of the scene requires a pointer to an instance of D3dManager
	~Scene();

};