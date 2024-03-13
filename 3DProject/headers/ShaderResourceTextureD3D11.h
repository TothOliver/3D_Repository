#pragma once

#include <d3d11_4.h>

class ShaderResourceTextureD3D11
{
private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;

public:
	ShaderResourceTextureD3D11() = default;
	ShaderResourceTextureD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData);
	ShaderResourceTextureD3D11(ID3D11Device* device, const char* pathToTextureFile);
	~ShaderResourceTextureD3D11();
	ShaderResourceTextureD3D11(const ShaderResourceTextureD3D11& other) = delete;
	ShaderResourceTextureD3D11& operator=(const ShaderResourceTextureD3D11& other) = delete;
	ShaderResourceTextureD3D11(ShaderResourceTextureD3D11&& other) = delete;
	ShaderResourceTextureD3D11& operator=(ShaderResourceTextureD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData);
	void Initialize(ID3D11Device* device, const char* pathToTextureFile);

	ID3D11ShaderResourceView* GetSRV() const;
};