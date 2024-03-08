#include "headers/ShaderResourceTextureD3D11.h"
#include "headers/stb_image.h"


ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D* texture)
{
}

ShaderResourceTextureD3D11::ShaderResourceTextureD3D11(ID3D11Device* device, const char* pathToTextureFile)
{
	Initialize(device, pathToTextureFile);
}

ShaderResourceTextureD3D11::~ShaderResourceTextureD3D11()
{
	if(this->texture != nullptr)
		this->texture->Release();
	if(this->srv != nullptr)
		this->srv->Release();
}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, ID3D11Texture2D* texture)
{
	this->texture = texture;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create depth buffer texture");
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = 1;

	ID3D11ShaderResourceView * srv;
	hr = device->CreateShaderResourceView(texture, &srvDesc, &srv);
	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create depth buffer srv");
	}

}

void ShaderResourceTextureD3D11::Initialize(ID3D11Device* device, const char* pathToTextureFile)
{
	
}

ID3D11ShaderResourceView* ShaderResourceTextureD3D11::GetSRV() const
{
	return this->srv;
}

ID3D11Texture2D* ShaderResourceTextureD3D11::GetTEXT() const
{
	return this->texture;
}
