#include "headers/DepthBufferD3D11.h"

DepthBufferD3D11::DepthBufferD3D11(ID3D11Device* device, UINT width, UINT height, bool hasSRV)
{
	Initialize(device, width, height, hasSRV);
}

DepthBufferD3D11::~DepthBufferD3D11()
{
	if (this->texture != nullptr)
		this->texture->Release();
	if (this->srv != nullptr)
		this->srv->Release();
	for (size_t i = 0; i < depthStencilViews.size(); i++) {
		depthStencilViews[i]->Release();

	}
}

void DepthBufferD3D11::Initialize(ID3D11Device* device, UINT width, UINT height, bool hasSRV, UINT arraySize)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = arraySize;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &this->texture))) {
		printf("ojoj");
	}


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice = 0;
	dsvDesc.Texture2DArray.ArraySize = 1;
	dsvDesc.Flags = 0;

	

	for (UINT dsvIndex = 0; dsvIndex < arraySize; ++dsvIndex)
	{
		ID3D11DepthStencilView* dsv;
		this->depthStencilViews.push_back(dsv);
		dsvDesc.Texture2DArray.FirstArraySlice = dsvIndex;

		if (FAILED(device->CreateDepthStencilView(this->texture, &dsvDesc, &this->depthStencilViews[dsvIndex]))){
			printf("wawa");
		}
	}

	if (!hasSRV) 
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		srvDesc.Texture2DArray.MostDetailedMip = 0;
		srvDesc.Texture2DArray.MipLevels = 1;
		srvDesc.Texture2DArray.FirstArraySlice = 0;
		srvDesc.Texture2DArray.ArraySize = arraySize;

		if (FAILED(device->CreateShaderResourceView(this->texture, &srvDesc, &this->srv))) {
			printf("wowo");
		}
	}

}

ID3D11DepthStencilView* DepthBufferD3D11::GetDSV(UINT arrayIndex) const
{
	return this->depthStencilViews[arrayIndex];
}

ID3D11ShaderResourceView* DepthBufferD3D11::GetSRV() const
{
	return this->srv;
}
