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

#include "headers/SpotLightCollectionD3D11.h"

#include "headers/Parser.h"
#include "headers/SceneManager.h"
#include <DirectXMath.h>

#define CAMERA_SPEED 0.05f
#define CAMERA_ROTATION_SPEED 0.005

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
	ID3D11Texture2D* dsTexture;
	ID3D11DepthStencilView* dsView;
	float clearColour[4] = { 0.3, 0.3, 0.3, 0 };
	float clear[4] = { 0, 0, 0, 1 };

	if (!SetupWindow(hInstance, WIDTH, HEIGHT, nCmdShow, window))
	{
		std::cerr << "Error: SetupWindow" << std::endl;
		return -1;
	}
	
	if (!SetupD3D11(WIDTH, HEIGHT, window, device, immediateContext, swapChain, uav, dsTexture, dsView, viewport))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}

	GBuffer positionBuffer(device, WIDTH, HEIGHT);
	GBuffer normalBuffer(device, WIDTH, HEIGHT);
	GBuffer ambientBuffer(device, WIDTH, HEIGHT);
	GBuffer diffuseBuffer(device, WIDTH, HEIGHT);
	GBuffer specularBuffer(device, WIDTH, HEIGHT);

	ID3D11RenderTargetView* rtv[5];
	ID3D11ShaderResourceView* srv[5];
	rtv[0] = positionBuffer.GetRTV();
	rtv[1] = normalBuffer.GetRTV();
	rtv[2] = ambientBuffer.GetRTV();
	rtv[3] = diffuseBuffer.GetRTV();
	rtv[4] = specularBuffer.GetRTV();

	srv[0] = positionBuffer.GetSRV();
	srv[1] = normalBuffer.GetSRV();
	srv[2] = ambientBuffer.GetSRV();
	srv[3] = diffuseBuffer.GetSRV();
	srv[4] = specularBuffer.GetSRV();

	ID3D11RenderTargetView* RTVnull[6];
	ID3D11ShaderResourceView* SRVnull[7]; //+1
	for (UINT i = 0; i < 6; i++)
		RTVnull[i] = nullptr;
	for (UINT i = 0; i < 7; i++)
		SRVnull[i] = nullptr;


	ShaderD3D11 vertexShader;
	ShaderD3D11 computeShader;
	ShaderD3D11 pixelShader;
	InputLayoutD3D11 inputLayout;
	SamplerD3D11 sampler;
	SamplerD3D11 samplerShadow; //+1
	std::string vShaderByteCode;
	

	//Initialize camera with desired values
	CameraD3D11 camera;
	camera.Initialize(device, { 1, (float)16 / 9, 0.05, 100 }, { 0, 2, -5.0f });

	//Initialize ligth sources with shadows
	SpotLightCollectionD3D11 directionalLight;
	SpotLightCollectionD3D11 spotLights;
	CreateLighths(device, directionalLight, spotLights);

	//Create buffer with number of lights for each light type
	NrOfLight NOL;
	NOL.nrOfSpotLights[0] = spotLights.GetNrOfLights();
	NOL.nrOfDirectionalLight[0] = directionalLight.GetNrOfLights();
	ConstantBufferD3D11 nrOfLightBuffer;
	nrOfLightBuffer.Initialize(device, sizeof(NOL), &NOL);
	ID3D11Buffer* nrBuffer = nrOfLightBuffer.GetBuffer();

	//Create variables for storing mouse input information and more
	ID3D11Buffer* vpBuffer;
	POINT p;
	RECT rect;
	LPRECT lpRect = &rect;
	ShowCursor(false);

	if (!ShaderLoader(device, immediateContext, vertexShader, computeShader, pixelShader, inputLayout, sampler, samplerShadow))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}


	std::vector<Scene*> scenes;
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

		vpBuffer = camera.GetVPBuffer();



		immediateContext->ClearRenderTargetView(rtv[0], clear);
		immediateContext->ClearRenderTargetView(rtv[1], clear);
		immediateContext->ClearRenderTargetView(rtv[2], clear);
		immediateContext->ClearRenderTargetView(rtv[3], clear);
		immediateContext->ClearRenderTargetView(rtv[4], clear);

		immediateContext->ClearUnorderedAccessViewFloat(uav, clearColour);
		immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);


		immediateContext->RSSetViewports(1, &viewport);
		immediateContext->IASetInputLayout(inputLayout.GetInputLayout());
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//shadow mapping
		vertexShader.BindShader(immediateContext);
		pixelShader.BindShader(immediateContext);
		ID3D11SamplerState* sState = sampler.GetSamplerState();
		ID3D11SamplerState* cState = samplerShadow.GetSamplerState();
		immediateContext->PSSetSamplers(0, 1, &sState);
		immediateContext->CSSetSamplers(0, 1, &cState);
		for (UINT lightIndex = 0; lightIndex < spotLights.GetNrOfLights(); lightIndex++) 
		{
			ID3D11DepthStencilView* dsv = spotLights.GetShadowMapDSV(lightIndex);
			immediateContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
			immediateContext->OMSetRenderTargets(5, RTVnull, dsv);

			ID3D11Buffer* cameraConstantBuffer = spotLights.GetLightCameraConstantBuffer(lightIndex);
			immediateContext->VSSetConstantBuffers(1, 1, &cameraConstantBuffer);

			scenes.at(sceneIndex)->DrawScene(immediateContext);
			immediateContext->OMSetRenderTargets(5, RTVnull, nullptr);
		}
		for (UINT lightIndex = 0; lightIndex < directionalLight.GetNrOfLights(); lightIndex++)
		{
			ID3D11DepthStencilView* dsv = directionalLight.GetShadowMapDSV(lightIndex);
			immediateContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
			immediateContext->OMSetRenderTargets(5, RTVnull, dsv);

			ID3D11Buffer* cameraConstantBuffer = directionalLight.GetLightCameraConstantBuffer(lightIndex);
			immediateContext->VSSetConstantBuffers(1, 1, &cameraConstantBuffer);

			scenes.at(sceneIndex)->DrawScene(immediateContext);
			immediateContext->OMSetRenderTargets(5, RTVnull, nullptr);
		}


		//final rendering pass
		immediateContext->VSSetConstantBuffers(1, 1, &vpBuffer);

		immediateContext->OMSetRenderTargets(5, rtv, dsView);

		scenes.at(sceneIndex)->DrawScene(immediateContext);

		immediateContext->OMSetRenderTargets(5, RTVnull, nullptr);

		immediateContext->CSSetShaderResources(0, 5, srv);

		//For shadow mapping
		ID3D11ShaderResourceView* srv3 = spotLights.GetShadowMapsSRV();
		ID3D11ShaderResourceView* srv2 = spotLights.GetLightBufferSRV();
		ID3D11ShaderResourceView* srv4 = directionalLight.GetLightBufferSRV();
		immediateContext->CSSetShaderResources(5, 1, &srv3);
		immediateContext->CSSetShaderResources(6, 1, &srv2);
		immediateContext->CSSetShaderResources(7, 1, &srv4);
		immediateContext->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);

		computeShader.BindShader(immediateContext);
		immediateContext->Dispatch(WIDTH / 8, HEIGHT / 8, 1);

		immediateContext->CSSetShaderResources(0, 7, SRVnull);

		ID3D11Buffer* cameraPosition = camera.GetPositionBuffer();
		immediateContext->CSSetConstantBuffers(0, 1, &cameraPosition);

		ID3D11Buffer* sBuffer = spotLights.GetLightBuffer();
		immediateContext->CSSetConstantBuffers(1, 1, &nrBuffer);


		if ((GetAsyncKeyState(0x31) & 0x0101)) //1 key
			sceneIndex--;
		if ((GetAsyncKeyState(0x32) & 0x0101)) //2 key
			sceneIndex++;


		if (sceneIndex < 0)
			sceneIndex = 0;
		if (sceneIndex >= scenes.size())
			sceneIndex = scenes.size() - 1;

		swapChain->Present(0, 0);
	}

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	dsTexture->Release();
	dsView->Release();
	uav->Release();

	for (size_t i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->~Scene();
	}

	return true;
}