#pragma once

#include <d3d11_4.h>

#include <vector>

class DepthBufferD3D11
{
private:
	ID3D11Texture2D* texture = nullptr;
	std::vector<ID3D11DepthStencilView*> depthStencilViews;
	ID3D11ShaderResourceView* srv = nullptr;

public:
	DepthBufferD3D11() = default;
	DepthBufferD3D11(ID3D11Device* device, UINT width, UINT height, bool hasSRV = false);
	~DepthBufferD3D11();
	DepthBufferD3D11(const DepthBufferD3D11& other) = delete;
	DepthBufferD3D11& operator=(const DepthBufferD3D11& other) = delete;
	DepthBufferD3D11(DepthBufferD3D11&& other) = delete;
	DepthBufferD3D11& operator=(DepthBufferD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT width, UINT height,
		bool hasSRV = false, UINT arraySize = 1);

	ID3D11DepthStencilView* GetDSV(UINT arrayIndex) const;
	ID3D11ShaderResourceView* GetSRV() const;
};