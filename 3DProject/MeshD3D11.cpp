#pragma once

#include "headers/Parser.h"
#include "headers/MeshD3D11.h"

using namespace DirectX;

MeshD3D11::MeshD3D11(ID3D11Device*& device, std::string filename, float worldX, float worldY, float worldZ, float scale)
{
	MeshData mData;
	ParseObj(device, filename, mData.vertexData, mData.indexData, this->subMeshes, this->mData.materials);

	this->vertexBuffer.Initialize(device, 11, mData.vertexData.size() / 11, mData.vertexData.data());

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixTranslation(worldX, worldY, worldZ)));
	this->worldMatrixBuffer.Initialize(device, sizeof(XMFLOAT4X4), &world);
}

MeshD3D11::~MeshD3D11()
{

	for (size_t i = 0; i < subMeshes.size(); i++)
	{
		delete subMeshes.at(i);
		subMeshes.at(i) = nullptr;
	}

	for (size_t i = 0; i < this->mData.materials.size(); i++)
	{
		this->mData.materials.at(i).ambientTexture->Release();
		this->mData.materials.at(i).diffuseTexture->Release();
		this->mData.materials.at(i).specularTexture->Release();
		this->mData.materials.at(i).emissionTexture->Release();
		this->mData.materials.at(i).surfaceMapping.normalTexture->Release();
	}
}

void MeshD3D11::BindMeshBuffers(ID3D11DeviceContext* context) const
{
	ID3D11Buffer* vBuffer = this->vertexBuffer.GetBuffer();
	UINT stride = this->vertexBuffer.GetVertexSize() * sizeof(float);
	UINT offset = 0;
	ID3D11Buffer* worldViewProj = this->worldMatrixBuffer.GetBuffer();

	context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	context->VSSetConstantBuffers(0, 1, &worldViewProj);



	for (size_t i = 0; i < subMeshes.size(); i++)
	{
		this->subMeshes.at(i)->PerformDrawCall(context);
	}
}

