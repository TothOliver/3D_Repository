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
	ID3D11DepthStencilView* dsv;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = arraySize;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	if (!hasSRV)
	{
		if (FAILED(device->CreateTexture2D(&depthStencilDesc, nullptr, &this->texture))) {
			printf("ojoj");
		}
		if (FAILED(device->CreateShaderResourceView(this->texture, nullptr, &srv))) {
			printf("ojaj");
		}
	}

	if (FAILED(device->CreateDepthStencilView(this->texture, nullptr, &dsv))) {
		printf("ajaj");
	}


	
	depthStencilViews.push_back(dsv);

}

ID3D11DepthStencilView* DepthBufferD3D11::GetDSV(UINT arrayIndex) const
{
	return this->depthStencilViews[arrayIndex];
}

ID3D11ShaderResourceView* DepthBufferD3D11::GetSRV() const
{
	return this->srv;
}
