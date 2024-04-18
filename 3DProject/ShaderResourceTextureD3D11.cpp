#include "headers/ShaderResourceD3D11.h"
#include "headers/stb_image.h"


ShaderResourceD3D11::ShaderResourceD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
	Initialize(device, width, height, textureData);
}

ShaderResourceD3D11::ShaderResourceD3D11(ID3D11Device* device, const char* pathToTextureFile)
{
	Initialize(device, pathToTextureFile);
}

ShaderResourceD3D11::~ShaderResourceD3D11()
{
	if (this->srv != nullptr)
		this->srv->Release();
}

void ShaderResourceD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
	ID3D11Texture2D* texture;

	D3D11_SUBRESOURCE_DATA texData;
	texData.pSysMem = textureData;
	texData.SysMemPitch = 4 * width;
	texData.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&texDesc, &texData, &texture))) {
		throw "Failed to create Texture";
	}

	if (FAILED(device->CreateShaderResourceView(texture, nullptr, &srv))) {
		throw "Failed to create ShaderResourceView";
	}

	texture->Release();
}

void ShaderResourceD3D11::Initialize(ID3D11Device* device, const char* pathToTextureFile)
{
	ID3D11Texture2D* texture;

	stbi_set_flip_vertically_on_load(true);

	int width;
	int height;
	int channels;
	int desireChannels = 4;

	unsigned char* imageData = stbi_load(pathToTextureFile, &width, &height, &channels, desireChannels);

	D3D11_SUBRESOURCE_DATA texData;
	texData.pSysMem = imageData;
	texData.SysMemPitch = 4 * width;
	texData.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&texDesc, &texData, &texture))) {
		throw "Failed to create Texture";
	}

	if (FAILED(device->CreateShaderResourceView(texture, nullptr, &srv))) {
		throw "Failed to create ShaderResourceView";
	}

	stbi_image_free(imageData);
	texture->Release();
}

ID3D11ShaderResourceView* ShaderResourceD3D11::GetSRV() const
{
	return this->srv;
}
