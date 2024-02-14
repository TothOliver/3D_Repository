#pragma once

#include <d3d11_4.h>
#include <iostream>

class RenderTargetD3D11
{
private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11RenderTargetView* rtv = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

public:
	RenderTargetD3D11() = default;
	~RenderTargetD3D11();
	RenderTargetD3D11(const RenderTargetD3D11& other) = delete;
	RenderTargetD3D11& operator=(const RenderTargetD3D11& other) = delete;
	RenderTargetD3D11(RenderTargetD3D11&& other) = delete;
	RenderTargetD3D11& operator=(RenderTargetD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT width, UINT height, IDXGISwapChain* swapChain,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, bool hasSRV = false);

	void CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain);


	ID3D11RenderTargetView* GetRTV() const;
	ID3D11ShaderResourceView* GetSRV() const;
};