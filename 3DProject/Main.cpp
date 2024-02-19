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


	while (!(GetKeyState(VK_ESCAPE) & 0x8000) && msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	device->Release();
	immediateContext->Release();
	swapChain->Release();
	rtv->Release();
	dsTexture->Release();
	dsView->Release();

	return true;
}