#pragma once

#include <d3d11_4.h>

class ShaderResourceD3D11
{
private:
	ID3D11ShaderResourceView* srv = nullptr;

public:
	ShaderResourceD3D11() = default;
	ShaderResourceD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData);
	ShaderResourceD3D11(ID3D11Device* device, const char* pathToTextureFile);
	~ShaderResourceD3D11();
	ShaderResourceD3D11(const ShaderResourceD3D11& other) = delete;
	ShaderResourceD3D11& operator=(const ShaderResourceD3D11& other) = delete;
	ShaderResourceD3D11(ShaderResourceD3D11&& other) = delete;
	ShaderResourceD3D11& operator=(ShaderResourceD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData);
	void Initialize(ID3D11Device* device, const char* pathToTextureFile);

	ID3D11ShaderResourceView* GetSRV() const;
};