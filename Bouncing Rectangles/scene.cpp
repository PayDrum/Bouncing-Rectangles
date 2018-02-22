#include "scene.hpp"

Scene::Scene(D3dManager* d3dManager)
{
	this->d3dManager = d3dManager;

	//Randomize the number of rectangles
	std::random_device rd;				//Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd());				//Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> numberOfRectanglesDistrib(minRectanglesNumber, maxRectanglesNumber);
	numberOfRectangles = numberOfRectanglesDistrib(gen);
}


HRESULT Scene::initGraphics()
{
	HRESULT hr = S_OK;

	for (int i = 0; i < numberOfRectangles; i++)
	{
		recVec.push_back(std::make_shared<Rect>());		//Create a new instance of Rect, put its shared pointer in our rectangle vector

		D3D11_BUFFER_DESC bd = {};

		bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bd.ByteWidth = sizeof(Rect::vertex) * 6;       // size is the VERTEX struct * 3
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = recVec[i]->vertices;
		hr = d3dManager->getDevice()->CreateBuffer(&bd, &InitData, &recVec[i]->vbo);       // create the buffer
		if (FAILED(hr))
			return hr;

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * 6;        // 6 vertices needed for 2 triangles in a triangle list
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		InitData.pSysMem = recVec[i]->indices;
		hr = d3dManager->getDevice()->CreateBuffer(&bd, &InitData, &recVec[i]->ibo);
		if (FAILED(hr))
			return hr;
		// Set index buffer
		d3dManager->getDeviceContext()->IASetIndexBuffer(recVec[i]->ibo, DXGI_FORMAT_R32_UINT, 0);


		//constant buffers
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(DirectX::XMMATRIX);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		hr = d3dManager->getDevice()->CreateBuffer(&bd, nullptr, &recVec[i]->cbo);
		if (FAILED(hr))
			return hr;
	}
	return hr;
}

HRESULT Scene::initShaders()
{
	//We load, compile and use only the first rectangle's shaders, since the shader is the same for all the rectangles
	return recVec[0]->shaders.load(d3dManager->getDevice(), d3dManager->getDeviceContext());
}

void Scene::update(FLOAT delta)
{
	//Scene update method
	for (int i = 0; i < numberOfRectangles; i++)
		recVec[i]->updateCurrentPosition(delta);
}

void Scene::render()
{
	//Clear the back buffer to a gray
	FLOAT clearColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
	d3dManager->getDeviceContext()->ClearRenderTargetView(d3dManager->getBackBuffer(), clearColor);

	for (int i = 0; i < numberOfRectangles; i++)
	{
		DirectX::XMMATRIX worldMat = recVec[i]->modelMat;
		worldMat = DirectX::XMMatrixTranspose(worldMat);
		d3dManager->getDeviceContext()->UpdateSubresource(recVec[i]->cbo, 0, nullptr, &worldMat, 0, 0);

		// set the shader objects
		d3dManager->getDeviceContext()->VSSetShader(recVec[0]->shaders.vertexShader, 0, 0);
		d3dManager->getDeviceContext()->VSSetConstantBuffers(0, 1, &recVec[i]->cbo);
		d3dManager->getDeviceContext()->PSSetShader(recVec[0]->shaders.pixelShader, 0, 0);

		// select which vertex buffer to display
		UINT stride = sizeof(Rect::vertex);
		UINT offset = 0;
		d3dManager->getDeviceContext()->IASetVertexBuffers(0, 1, &recVec[i]->vbo, &stride, &offset);

		// select which primtive type we are using
		d3dManager->getDeviceContext()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		// draw the vertex buffer to the back buffer
		d3dManager->getDeviceContext()->DrawIndexed(6, 0, 0);
	}

	// switch the back buffer and the front buffer
	d3dManager->getSwapchain()->Present(0, 0);

}

Scene::~Scene()
{
	//cleanup
	recVec[0]->shaders.layout->Release();
	recVec[0]->shaders.vertexShader->Release();
	recVec[0]->shaders.pixelShader->Release();
	recVec.clear();
}