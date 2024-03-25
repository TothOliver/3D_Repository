#pragma once

#include "headers/SubMeshD3D11.h"
#include "headers/Parser.h"
#include "headers/stb_image.h"
#include <numeric>

SubMeshD3D11::SubMeshD3D11(Material mat, size_t startIndex, size_t nrOfIndices)
{
	this->startIndex = startIndex;
	this->nrOfIndices = nrOfIndices;
	this->mat = mat;
}

void SubMeshD3D11::Initialize(ID3D11Device* device)
{
}

void SubMeshD3D11::PerformDrawCall(ID3D11DeviceContext* immediateContext) const
{

	ID3D11ShaderResourceView* srvs[4];
	srvs[0] = this->mat.ambientTexture;
	srvs[1] = this->mat.diffuseTexture;
	srvs[2] = this->mat.specularTexture;
	srvs[3] = this->mat.surfaceMapping.normalTexture;

	immediateContext->PSSetShaderResources(2, 4, srvs);

	immediateContext->Draw(this->nrOfIndices, this->startIndex);
}

ID3D11ShaderResourceView* SubMeshD3D11::GetAmbientSRV() const { return this->mat.ambientTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDiffuseSRV() const { return this->mat.diffuseTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetSpecularSRV() const { return this->mat.specularTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetEmissionSRV() const { return this->mat.emissionTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDisplacementSRV() const { return this->mat.surfaceMapping.displacementTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetNormalSRV() const { return this->mat.surfaceMapping.normalTexture; }