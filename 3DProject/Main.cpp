#pragma once

#include <Windows.h>
#include <iostream>
#include <d3d11.h>

#include "headers/Window.h"
#include "headers/D3D11Helper.h"

#include "headers/ShaderLoader.h"
#include "headers/InputLayoutD3D11.h"
#include "headers/SamplerD3D11.h"
#include "headers/CameraD3D11.h"
#include "headers/GBuffer.h"

#include "headers/Parser.h"
#include "headers/SceneManager.h"
#include <DirectXMath.h>
#include "headers/Render.h"
#include "headers/TextureCube.h"
#include <chrono>

#define CAMERA_SPEED 6
#define CAMERA_ROTATION_SPEED 0.01
#define REFLECTION_RESOLUTION 512

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
	ID3D11UnorderedAccessView* uav = {};
	ID3D11DepthStencilView* dsView;

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Error: SetupWindow" << std::endl;
		return -1;
	}
	
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, uav, dsView, viewport))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}

	ShaderD3D11 vertexShader;
	ShaderD3D11 computeShader;
	InputLayoutD3D11 inputLayout;
	SamplerD3D11 textureSampler;
	SamplerD3D11 shadowSampler;

	ShaderD3D11 ps[2];
	ShaderD3D11 particleShaders[4];

	//Initialize camera with desired values
	CameraD3D11 camera;
	camera.Initialize(device, { 1, (float)16 / 9, 0.05, 100 }, { 0, 0, 0 });

	//Create variables for storing mouse input information and more
	ID3D11Buffer* vpBuffer;
	POINT p;
	RECT rect;
	LPRECT lpRect = &rect;
	ShowCursor(false);
	GetWindowRect(window, lpRect);

	if (!ShaderLoader(device, immediateContext, vertexShader, computeShader, ps, particleShaders, inputLayout, textureSampler, shadowSampler))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}

	std::vector<Scene*> scenes;
	CreateScenes(device, scenes);
	int sceneIndex = 2;

	float deltaTime = 1;

	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		auto t0 = std::chrono::system_clock::now();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (GetAsyncKeyState(0x57) & 0x8000) // W key
		{
			camera.MoveForward(CAMERA_SPEED * deltaTime);
		}
		if (GetAsyncKeyState(0x53) & 0x8000) // S key
		{
			camera.MoveForward(-CAMERA_SPEED * deltaTime);
		}
		if (GetAsyncKeyState(0x41) & 0x8000) // A key
		{
			camera.MoveRight(-CAMERA_SPEED * deltaTime);
		}
		if (GetAsyncKeyState(0x44) & 0x8000)
		{
			camera.MoveRight(CAMERA_SPEED * deltaTime); // D key
		}
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			camera.MoveUp(CAMERA_SPEED * deltaTime); // Space
		}
		if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		{
			camera.MoveUp(-CAMERA_SPEED * deltaTime); //Shift
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


		Scene* currentScene = scenes.at(sceneIndex);

		//Update particles
		particleShaders[0].BindShader(immediateContext);

		for (size_t i = 0; i < currentScene->GetEmitterCount(); i++)
		{
			ID3D11UnorderedAccessView* particleUAV = currentScene->GetEmitterAt(i)->GetBufferUAV();
			immediateContext->CSSetUnorderedAccessViews(0, 1, &particleUAV, nullptr);
			immediateContext->Dispatch(std::ceil(currentScene->GetEmitterAt(i)->GetNrOfParticles() / 32.0f), 1, 1);
		}
		
		ID3D11UnorderedAccessView* UAVnull = nullptr;
		immediateContext->CSSetUnorderedAccessViews(0, 1, &UAVnull, nullptr);

		//Updating camera position
		camera.UpdateInternalConstantBuffer(immediateContext);

		//shadow mapping
		vertexShader.BindShader(immediateContext);
		ID3D11SamplerState* cState = shadowSampler.GetSamplerState();
		immediateContext->CSSetSamplers(0, 1, &cState);

		immediateContext->RSSetViewports(1, &viewport);
		ID3D11InputLayout* il = inputLayout.GetInputLayout();
		immediateContext->IASetInputLayout(il);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//Render shadow maps for spotlights
		for (UINT lightIndex = 0; lightIndex < currentScene->GetNrOfLights(SPOTLIGHT); lightIndex++)
		{
			ID3D11DepthStencilView* dsv = currentScene->GetShadowMapDSV(SPOTLIGHT, lightIndex);
			immediateContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
			immediateContext->OMSetRenderTargets(0, nullptr, dsv);

			ID3D11Buffer* cameraConstantBuffer = currentScene->GetLightCameraConstantBuffer(SPOTLIGHT, lightIndex);
			immediateContext->VSSetConstantBuffers(1, 1, &cameraConstantBuffer);

			currentScene->DrawScene(immediateContext, nullptr, true);

			dsv = nullptr;
			immediateContext->OMSetRenderTargets(0, nullptr, dsv);
		}

		//Render shadow maps for directional lights
		for (UINT lightIndex = 0; lightIndex < scenes.at(sceneIndex)->GetNrOfLights(DIRECTIONAL); lightIndex++)
		{
			ID3D11DepthStencilView* dsv = currentScene->GetShadowMapDSV(DIRECTIONAL, lightIndex);
			immediateContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
			immediateContext->OMSetRenderTargets(0, nullptr, dsv);

			ID3D11Buffer* cameraConstantBuffer = currentScene->GetLightCameraConstantBuffer(DIRECTIONAL, lightIndex); //new
			immediateContext->VSSetConstantBuffers(1, 1, &cameraConstantBuffer);

			currentScene->DrawScene(immediateContext, nullptr, true);

			dsv = nullptr;
			immediateContext->OMSetRenderTargets(0, nullptr, dsv);
		}

		computeShader.BindShader(immediateContext);

		//Cubemap rendering
		for (size_t i = 0; i < scenes.at(sceneIndex)->GetNrOfMeshes(); i++)
		{
			MeshD3D11* mesh = scenes.at(sceneIndex)->GetMeshAt(i);
			if (mesh->IsReflective())
			{
				TextureCube* cubeMap = mesh->GetTextureCube();

				//Iterate for each side in cubemap
				for (size_t side = 0; side < 6; side++)
				{
					RenderToTargetUAV(device, immediateContext, cubeMap->GetCubeUAV(side), cubeMap->GetCubeSideLength(), cubeMap->GetCubeSideLength(),
						cubeMap->GetCubeDSV(), inputLayout.GetInputLayout(), textureSampler.GetSamplerState(), cubeMap->GetCubeViewport(), cubeMap->GetCameraVPBuffer(side),
						cubeMap->GetCameraPOSBuffer(side), currentScene, 1, vertexShader, computeShader, ps, particleShaders);
				}
			}
		}

		//Render scene at sceneIndex
		RenderToTargetUAV(device, immediateContext, uav, WIDTH, HEIGHT, dsView, inputLayout.GetInputLayout(), 
			textureSampler.GetSamplerState(), viewport, camera.GetVPBuffer(), camera.GetPositionBuffer(), currentScene, 0, vertexShader, computeShader, ps, particleShaders);

		immediateContext->PSSetShader(nullptr, nullptr, 0);

		if ((GetAsyncKeyState(0x31) & 0x0101)) //1 key
			sceneIndex--;
		if ((GetAsyncKeyState(0x32) & 0x0101)) //2 key
			sceneIndex++;

		if (sceneIndex < 0)
			sceneIndex = 0;
		if (sceneIndex >= scenes.size())
			sceneIndex = scenes.size() - 1;

		swapChain->Present(0, 0);

		auto t1 = std::chrono::system_clock::now();
		auto float_secs = std::chrono::duration_cast<std::chrono::duration<float>>(t1 - t0);
		deltaTime = float_secs.count();
	}

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	dsView->Release();
	uav->Release();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->~Scene();
	}

	return true;
}