#include "headers/SamplerD3D11.h"

SamplerD3D11::SamplerD3D11(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode, std::optional<std::array<float, 4>> borderColour)
{
	Initialize(device, adressMode, borderColour);
}

SamplerD3D11::~SamplerD3D11()
{
	this->samplerState->Release();
}

void SamplerD3D11::Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode, std::optional<std::array<float, 4>> borderColour, D3D11_FILTER filter, D3D11_COMPARISON_FUNC comparisonFunc)
{
	D3D11_SAMPLER_DESC sampler;
	sampler.Filter = filter;
	sampler.AddressU = adressMode;
	sampler.AddressV = adressMode;
	sampler.AddressW = adressMode;
	sampler.MipLODBias = 1;
	sampler.MaxAnisotropy = 16;
	sampler.ComparisonFunc = comparisonFunc;
	sampler.BorderColor[0] = borderColour.value()[0];
	sampler.BorderColor[1] = borderColour.value()[1];
	sampler.BorderColor[2] = borderColour.value()[2];
	sampler.BorderColor[3] = borderColour.value()[3];
	sampler.MinLOD = 0;
	sampler.MaxLOD = 0;

	device->CreateSamplerState(&sampler, &samplerState);
}


ID3D11SamplerState* SamplerD3D11::GetSamplerState() const
{
	return this->samplerState;
}
