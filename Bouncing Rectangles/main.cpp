// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <chrono>

#include "d3dmanager.hpp"
#include "scene.hpp"
#include "settings.hpp"

//Main message handler 
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//Application Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc = {};

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, Settings::getInstance().screenWidth,Settings::getInstance().screenHeight};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		"Bouncing Rectangles",
		(WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX) ,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	//Initialize Direct3d
	D3dManager d3dManager;
	if (d3dManager.init(hWnd) != S_OK)
		return 0;

	//Load the scene
	Scene scene(&d3dManager);
	if (scene.initGraphics() != S_OK)
		return 0;
	if (scene.initShaders() != S_OK)
		return 0;


	//Timer variables
	auto curTime = std::chrono::high_resolution_clock::now();
	auto prevTime = std::chrono::high_resolution_clock::now();

	MSG msg;
	while (TRUE)			//Main loop:
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		
		//Update the scene based on the clock
		curTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> delta = curTime - prevTime;
		scene.update(delta.count());
		prevTime = curTime;

		//Render one frame
		scene.render();
	}

	return msg.wParam;
}




