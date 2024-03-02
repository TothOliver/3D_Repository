#pragma once

#include "headers/SubMeshD3D11.h"
#include "headers/Parser.h"
#include "headers/stb_image.h"

SubMeshD3D11::SubMeshD3D11(Material mat, size_t startIndex, size_t nrOfIndicies)
{
	this->startIndex = startIndex;
	this->nrOfIndices = nrOfIndices;
	this->mat = mat;
}

SubMeshD3D11::~SubMeshD3D11()
{
	/*
	mat.ambientTexture->Release();
	mat.diffuseTexture->Release();
	mat.specularTexture->Release();
	mat.emissionTexture->Release();
	*/
	/*if (mat.surfaceMapping.displacementTexture != nullptr)
		mat.surfaceMapping.displacementTexture->Release();
	else if (mat.surfaceMapping.normalTexture != nullptr)
		mat.surfaceMapping.normalTexture->Release();*/
}

void SubMeshD3D11::PerformDrawCall(ID3D11DeviceContext* immediateContext) const
{
	//immediateContext->IASetVertexBuffer();
	//immediateContext->IASetIndexBuffer();

	ID3D11ShaderResourceView* srvs;
	/*
	srvs[0] = this->mat.ambientTexture;
	srvs[1] = this->mat.diffuseTexture;
	srvs[2] = this->mat.specularTexture;
	srvs[3] = this->mat.emissionTexture;

	if (this->mat.surfaceMapping.normalTexture != nullptr)
		srvs[4] = this->mat.surfaceMapping.normalTexture;
	else
		srvs[4] = mat.surfaceMapping.displacementTexture;*/

	srvs = this->mat.ambientTexture;
	if (this->mat.ambientTexture == nullptr)
	{
		unsigned char* textureData;
		int width, height, channels;
		ID3D11Device* device;
		immediateContext->GetDevice(&device);
		textureData = stbi_load("assets/default.png", &width, &height, &channels, 4);
		srvs = CreateSRV(device, textureData, width, height);
	}
	

	immediateContext->CSSetShaderResources(2, 1, &srvs);
}

ID3D11ShaderResourceView* SubMeshD3D11::GetAmbientSRV() const { return this->mat.ambientTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDiffuseSRV() const { return this->mat.diffuseTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetSpecularSRV() const { return this->mat.specularTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetEmissionSRV() const { return this->mat.emissionTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDisplacementSRV() const { return this->mat.surfaceMapping.displacementTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetNormalSRV() const { return this->mat.surfaceMapping.normalTexture; }