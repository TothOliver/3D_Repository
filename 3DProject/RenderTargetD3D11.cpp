#include "headers/RenderTargetD3D11.h"


RenderTargetD3D11::~RenderTargetD3D11()
{
	this->rtv->Release();
}

void RenderTargetD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, IDXGISwapChain* swapChain, DXGI_FORMAT format, bool hasSRV)
{
	CreateRenderTargetView(device, swapChain);
	
	if (hasSRV) {
		//CreateShaderResourceView();
	}
}

void RenderTargetD3D11::CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain)
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (!FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		device->CreateRenderTargetView(backBuffer, NULL, &rtv);
	}
	backBuffer->Release();
}


ID3D11RenderTargetView* RenderTargetD3D11::GetRTV() const
{
	return this->rtv;
}

ID3D11ShaderResourceView* RenderTargetD3D11::GetSRV() const
{
	return this->srv;
}
