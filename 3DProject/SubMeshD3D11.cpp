#pragma once

#include "SubMeshD3D11.h"

SubMeshD3D11::SubMeshD3D11(Material mat, size_t startIndex, size_t nrOfIndicies)
{
	this->startIndex = startIndex;
	this->nrOfIndices = nrOfIndices;
	this->mat = mat;
}

SubMeshD3D11::~SubMeshD3D11()
{
	mat.ambientTexture->Release();
	mat.diffuseTexture->Release();
	mat.specularTexture->Release();
	mat.normalTexture->Release();
	mat.displacementTexture->Release();
	mat.parallaxTexture->Release();
	mat.emissionTexture->Release();
}

ID3D11ShaderResourceView* SubMeshD3D11::GetAmbientSRV() const { return this->mat.ambientTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDiffuseSRV() const { return this->mat.diffuseTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetSpecularSRV() const { return this->mat.specularTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetEmissionSRV() const { return this->mat.emissionTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDisplacementSRV() const { return this->mat.displacementTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetParallaxSRV() const { return this->mat.parallaxTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetNormalSRV() const { return this->mat.normalTexture; }