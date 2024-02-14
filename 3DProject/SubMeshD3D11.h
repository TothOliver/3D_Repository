#pragma once

#include <string>

#include <d3d11_4.h>

class SubMeshD3D11
{
private:
	size_t startIndex = 0;
	size_t nrOfIndices = 0;

	ID3D11ShaderResourceView* ambientTexture = nullptr;
	ID3D11ShaderResourceView* diffuseTexture = nullptr;
	ID3D11ShaderResourceView* specularTexture = nullptr;

public:
	SubMeshD3D11() = default;
	~SubMeshD3D11() = default;
	SubMeshD3D11(const SubMeshD3D11& other) = default;
	SubMeshD3D11& operator=(const SubMeshD3D11& other) = default;
	SubMeshD3D11(SubMeshD3D11&& other) = default;
	SubMeshD3D11& operator=(SubMeshD3D11&& other) = default;

	void Initialize(size_t startIndexValue, size_t nrOfIndicesInSubMesh,
		ID3D11ShaderResourceView* ambientTextureSRV, ID3D11ShaderResourceView* diffuseTextureSRV,
		ID3D11ShaderResourceView* specularTextureSRV);

	void PerformDrawCall(ID3D11DeviceContext* context) const;

	ID3D11ShaderResourceView* GetAmbientSRV() const;
	ID3D11ShaderResourceView* GetDiffuseSRV() const;
	ID3D11ShaderResourceView* GetSpecularSRV() const;
};