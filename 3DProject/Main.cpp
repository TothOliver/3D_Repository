#pragma once

#include <Windows.h>
#include <iostream>
#include <d3d11.h>

#include "Window.h"
#include "D3D11Helper.h"
//#include "ShaderD3D11.h"

#include "ShaderLoader.h"
#include "InputLayoutD3D11.h"
#include "SamplerD3D11.h"
#include "ShaderResourceTextureD3D11.h"
#include "CameraD3D11.h"

#include "Parser.h"
#include "MeshD3D11.h"
#include <DirectXMath.h>

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

	

	if (!ShaderLoader(device, immediateContext, vertexShader, pixelShader, inputLayout, srt, sampler))
	{
		std::cerr << "Error: SetupD3D11" << std::endl;
		return -1;
	}

	std::vector<float> vBuffer;
	std::vector<uint32_t> iBuffer;
	std::vector<SubMeshD3D11> sMeshes;

	ParseObj(device, "teapot.obj", vBuffer, iBuffer, sMeshes);

	ID3D11Buffer* matrixBuffer;
	DirectX::XMFLOAT4X4 matricies[2];

	////Create world matrix and store in buffer
	DirectX::XMStoreFloat4x4(&matricies[0], XMMatrixTranslation(0, 0, 0));

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH({ 0, 0, -10 }, { 0, 0, 1 }, { 0, 1, 0 });

	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(1, (float)16 / 9, 1, 20);

	////Store viewxprojection matrix beside world matrix in memory
	DirectX::XMStoreFloat4x4(&matricies[1], DirectX::XMMatrixMultiply(view, projection));

	D3D11_BUFFER_DESC matrixbufferdesc;
	matrixbufferdesc.ByteWidth = sizeof(DirectX::XMFLOAT4X4) * 2;
	matrixbufferdesc.Usage = D3D11_USAGE_DEFAULT;
	matrixbufferdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixbufferdesc.CPUAccessFlags = 0;
	matrixbufferdesc.MiscFlags = 0;
	matrixbufferdesc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA matrixdata;
	matrixdata.pSysMem = &matricies;
	matrixdata.SysMemPitch = 0;
	matrixdata.SysMemSlicePitch = 0;
	device->CreateBuffer(&matrixbufferdesc, &matrixdata, &matrixBuffer);

	ID3D11Buffer* vertexB;
	D3D11_BUFFER_DESC vDesc;
	vDesc.ByteWidth = sizeof(float) * vBuffer.size();
	vDesc.Usage = D3D11_USAGE_DEFAULT;
	vDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vDesc.CPUAccessFlags = 0;
	vDesc.MiscFlags = 0;
	vDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vBuffer.data();
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	device->CreateBuffer(&vDesc, &subData, &vertexB);

	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UINT stride = 32;
		UINT offset = 0;

		float clearColour[4] = { 0.3, 0.3, 0.3, 0 };
		immediateContext->ClearRenderTargetView(rtv, clearColour);
		immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
		
		immediateContext->IASetInputLayout(inputLayout.GetInputLayout());
		immediateContext->IASetVertexBuffers(0, 1, &vertexB, &stride, &offset);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		immediateContext->VSSetConstantBuffers(0, 1, &matrixBuffer);

		vertexShader.BindShader(immediateContext);

		immediateContext->RSSetViewports(1, &viewport);

		pixelShader.BindShader(immediateContext);

		immediateContext->OMSetRenderTargets(1, &rtv, dsView);

		immediateContext->Draw(vBuffer.size() / 8, 0);

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