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

SubMeshD3D11::~SubMeshD3D11()
{
	this->specularExponentBuffer->Release();
}

void SubMeshD3D11::Initialize(ID3D11Device* device)
{
	D3D11_BUFFER_DESC bDesc;
	bDesc.ByteWidth = 4 * sizeof(float);
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bDesc.CPUAccessFlags = 0;
	bDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bData;
	bData.pSysMem = &this->mat.specularExponent;
	bData.SysMemPitch = 0;
	bData.SysMemSlicePitch = 0;

	device->CreateBuffer(&bDesc, &bData, &this->specularExponentBuffer);
}

void SubMeshD3D11::PerformDrawCall(ID3D11DeviceContext* immediateContext, bool displace) const
{

	ID3D11ShaderResourceView* srvs[5];
	srvs[0] = this->mat.ambientTexture;
	srvs[1] = this->mat.diffuseTexture;
	srvs[2] = this->mat.specularTexture;
	srvs[3] = this->mat.normalTexture;
	srvs[4] = this->mat.heightTexture;

	//Will be nullptr if displacement mapping is enabled
	ID3D11ShaderResourceView* heightSRV = this->mat.heightTexture;
	
	immediateContext->PSSetShaderResources(0, 5, srvs);

	if (displace) {
		ID3D11ShaderResourceView* null = nullptr;
		immediateContext->PSSetShaderResources(4, 1, &null);
	}

	ID3D11Buffer* buf = this->specularExponentBuffer;
	immediateContext->PSSetConstantBuffers(1, 1, &buf);

	immediateContext->Draw(this->nrOfIndices, this->startIndex);
}


ID3D11ShaderResourceView* SubMeshD3D11::GetAmbientSRV() const { return this->mat.ambientTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetDiffuseSRV() const { return this->mat.diffuseTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetSpecularSRV() const { return this->mat.specularTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetHeightSRV() const { return this->mat.heightTexture; }

ID3D11ShaderResourceView* SubMeshD3D11::GetNormalSRV() const { return this->mat.normalTexture; }