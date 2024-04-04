#pragma once

#include <optional> // Needs C++ 17 or higher
#include <array>

#include <d3d11_4.h>

class SamplerD3D11
{
private:
	ID3D11SamplerState* samplerState = nullptr;

public:
	SamplerD3D11() = default;
	SamplerD3D11(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode,
		std::optional<std::array<float, 4>> borderColour = std::nullopt);
	~SamplerD3D11();
	SamplerD3D11(const SamplerD3D11& other) = delete;
	SamplerD3D11& operator=(const SamplerD3D11& other) = delete;
	SamplerD3D11(SamplerD3D11&& other) = delete;
	SamplerD3D11& operator=(SamplerD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, D3D11_TEXTURE_ADDRESS_MODE adressMode, std::optional<std::array<float, 4>> borderColour = std::nullopt,
		D3D11_FILTER filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR, D3D11_COMPARISON_FUNC comparisonFunc = D3D11_COMPARISON_NEVER);

	ID3D11SamplerState* GetSamplerState() const;
};