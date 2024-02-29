#pragma once

#include <Windows.h>
#include <iostream>
#include <d3d11.h>

#include "headers/Window.h"
#include "headers/D3D11Helper.h"

#include "headers/ShaderLoader.h"
#include "headers/InputLayoutD3D11.h"
#include "headers/SamplerD3D11.h"
#include "headers/ShaderResourceTextureD3D11.h"
#include "headers/CameraD3D11.h"

#include "headers/Parser.h"
#include "headers/SceneManager.h"
#include <DirectXMath.h>

#define CAMERA_SPEED 0.002f
#define CAMERA_ROTATION_SPEED 0.002

using namespace DirectX;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	const UINT WIDTH = 1024;
	const UINT HEIGHT = 576;
	HWND window;
	MSG msg = { };

	D3D11_VIEWPORT viewport;
	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* rtv;
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Error: SetupWindow" << std::endl;
		return -1;
	}
	
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, rtv, dsTexture, dsView, viewport))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}


	ShaderD3D11 vertexShader;
	ShaderD3D11 pixelShader;
	InputLayoutD3D11 inputLayout;
	ShaderResourceTextureD3D11 srt;
	SamplerD3D11 sampler;

	std::string vShaderByteCode;

	//Initialize camera with desired values
	CameraD3D11 camera;
	camera.Initialize(device, { 1, (float)16 / 9, 1, 100 }, { 0, 0, -10.0f });

	//Create variables for storing mouse input information and more
	ID3D11Buffer* vpBuffer;
	POINT p;
	RECT rect;
	LPRECT lpRect = &rect;
	ShowCursor(false);

	if (!ShaderLoader(device, immediateContext, vertexShader, pixelShader, inputLayout, srt, sampler))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}

	std::vector<Scene> scenes;
	CreateScenes(device, scenes);
	int sceneIndex = 0;

	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GetWindowRect(window, lpRect);

		if (GetAsyncKeyState(0x57) & 0x8000) // W key
		{
			camera.MoveForward(CAMERA_SPEED);
		}
		if (GetAsyncKeyState(0x53) & 0x8000) // S key
		{
			camera.MoveForward(-CAMERA_SPEED);
		}
		if (GetAsyncKeyState(0x41) & 0x8000) // A key
		{
			camera.MoveRight(-CAMERA_SPEED);
		}
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			camera.MoveRight(CAMERA_SPEED); // D key
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			camera.MoveUp(CAMERA_SPEED); // Space
		}
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			camera.MoveUp(-CAMERA_SPEED); //Shift
		}


		if (GetCursorPos(&p))
		{
			float windowCenterX = rect.left + (rect.right - rect.left) / 2;
			float windowCenterY = rect.top + (rect.bottom - rect.top) / 2;

			if (p.x > windowCenterX + 2)
			{
				camera.RotateUp((p.x - windowCenterX) * CAMERA_ROTATION_SPEED);
			}
			if (p.x < windowCenterX - 2)
			{
				camera.RotateUp((p.x - windowCenterX) * CAMERA_ROTATION_SPEED);
			}
			if (p.y > windowCenterY + 2)
			{
				camera.RotateRight((p.y - windowCenterY) * CAMERA_ROTATION_SPEED);
			}
			else if (p.y < windowCenterY - 2)
			{

				camera.RotateRight((p.y - windowCenterY) * CAMERA_ROTATION_SPEED);
			}

			SetCursorPos(rect.left + (rect.right - rect.left) / 2, rect.top + (rect.bottom - rect.top) / 2);
		}

		//Updating camera position
		camera.UpdateInternalConstantBuffer(immediateContext);

		vpBuffer = camera.GetConstantBuffer();

		immediateContext->VSSetConstantBuffers(1, 1, &vpBuffer);

		float clearColour[4] = { 0.3, 0.3, 0.3, 0 };
		immediateContext->ClearRenderTargetView(rtv, clearColour);
		immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		
		immediateContext->IASetInputLayout(inputLayout.GetInputLayout());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11SamplerState* sState = sampler.GetSamplerState();
		immediateContext->PSSetSamplers(0, 1, &sState);

		vertexShader.BindShader(immediateContext);

		immediateContext->RSSetViewports(1, &viewport);

		pixelShader.BindShader(immediateContext);

		immediateContext->OMSetRenderTargets(1, &rtv, dsView);

		scenes.at(sceneIndex).DrawScene(immediateContext);

		if ((GetAsyncKeyState(0x31) & 0x0101)) //A key
			sceneIndex--;
		if ((GetAsyncKeyState(0x32) & 0x0101)) //D key
			sceneIndex++;


		if ((GetAsyncKeyState(0x50) & 0x0101)) //P key


		if (sceneIndex < 0)
			sceneIndex = 0;
		if (sceneIndex >= scenes.size())
			sceneIndex = scenes.size() - 1;

		swapChain->Present(0, 0);
	}

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	rtv->Release();
	dsTexture->Release();
	dsView->Release();

	return true;
}