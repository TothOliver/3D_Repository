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

	float specularExponent = 1;

public:
	SubMeshD3D11(Material mat, size_t startIndex, size_t nrOfIndicies);
	~SubMeshD3D11();
	SubMeshD3D11(const SubMeshD3D11& other) = default;
	SubMeshD3D11& operator=(const SubMeshD3D11& other) = default;
	SubMeshD3D11(SubMeshD3D11&& other) = default;
	SubMeshD3D11& operator=(SubMeshD3D11&& other) = default;

	void Initialize(size_t startIndexValue, size_t nrOfIndicesInSubMesh,
		ID3D11ShaderResourceView* ambientTextureSRV, ID3D11ShaderResourceView* diffuseTextureSRV,
		ID3D11ShaderResourceView* specularTextureSRV) = delete;

	void PerformDrawCall(ID3D11DeviceContext* context) const;

	ID3D11ShaderResourceView* GetAmbientSRV() const;
	ID3D11ShaderResourceView* GetDiffuseSRV() const;
	ID3D11ShaderResourceView* GetSpecularSRV() const;
	ID3D11ShaderResourceView* GetEmissionSRV() const;
	ID3D11ShaderResourceView* GetDisplacementSRV() const;
	ID3D11ShaderResourceView* GetParallaxSRV() const;
	ID3D11ShaderResourceView* GetNormalSRV() const;
};
