#include "ShaderResourceTextureD3D11.h"
#include "stb_image.h"


ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
}

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, const char* pathToTextureFile)
{
	Initialize(device, pathToTextureFile);
}

ShaderResourceTextureD3D11::~ShaderResourceTextureD3D11()
{
	//this->texture->Release();
	//this->srv->Release();
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, void* textureData)
{
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, const char* pathToTextureFile)
{
	int width;
	int height;
	int channels;
	int desireChannels = 4;

	unsigned char* image = stbi_load(pathToTextureFile, &width, &height, &channels, desireChannels);

	D3D11_SUBRESOURCE_DATA imagedata;
	imagedata.pSysMem = image;
	imagedata.SysMemPitch = desireChannels * width;
	imagedata.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC texture;
	texture.Width = width;
	texture.Height = height;
	texture.MipLevels = 1;
	texture.ArraySize = 1;
	texture.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture.SampleDesc.Count = 1;
	texture.SampleDesc.Quality = 0;
	texture.Usage = D3D11_USAGE_IMMUTABLE;
	texture.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture.CPUAccessFlags = 0;
	texture.MiscFlags = 0;

	device->CreateTexture2D(&texture, &imagedata, &this->texture);

	if(this->texture != 0)
		device->CreateShaderResourceView(this->texture, nullptr, &srv);
}

ID3D11ShaderResourceView* ShaderResourceTextureD3D11::GetSRV() const
{
	return this->srv;
}
