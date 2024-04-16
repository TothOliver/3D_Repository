#pragma once

#include <string>
#include <d3d11_4.h>
#include "Material.h"

class SubMeshD3D11
{
private:
	size_t startIndex = 0;
	size_t nrOfIndices = 0;

	Material mat;

	ID3D11Buffer* specularExponentBuffer;

public:
	SubMeshD3D11(Material mat, size_t startIndex, size_t nrOfIndicies);
	~SubMeshD3D11();
	SubMeshD3D11(const SubMeshD3D11& other) = default;
	SubMeshD3D11& operator=(const SubMeshD3D11& other) = default;
	SubMeshD3D11(SubMeshD3D11&& other) = default;
	SubMeshD3D11& operator=(SubMeshD3D11&& other) = default;

	void Initialize(ID3D11Device* device);

	void PerformDrawCall(ID3D11DeviceContext* context) const;

	ID3D11ShaderResourceView* GetAmbientSRV() const;
	ID3D11ShaderResourceView* GetDiffuseSRV() const;
	ID3D11ShaderResourceView* GetSpecularSRV() const;
	ID3D11ShaderResourceView* GetHeightSRV() const;
	ID3D11ShaderResourceView* GetNormalSRV() const;
};
