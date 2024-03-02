#pragma once

#include "headers/Parser.h"
#include "headers/MeshD3D11.h"

using namespace DirectX;

MeshD3D11::MeshD3D11(ID3D11Device*& device, std::string filename, float worldX, float worldY, float worldZ, float scale)
{
	MeshData mData;
	ParseObj(device, filename, mData.vertexData, mData.indexData, this->subMeshes);

	this->vertexBuffer.Initialize(device, 8, mData.vertexData.size() / 8, mData.vertexData.data());

	this->indexBuffer.Initialize(device, mData.indexData.size(), mData.indexData.data());

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixTranslation(worldX, worldY, worldZ)));
	this->worldMatrixBuffer.Initialize(device, sizeof(XMFLOAT4X4), &world);
}

void MeshD3D11::BindMeshBuffers(ID3D11DeviceContext* context) const
{
	ID3D11Buffer* vBuffer = this->vertexBuffer.GetBuffer();
	UINT stride = this->vertexBuffer.GetVertexSize() * sizeof(float);
	UINT offset = 0;
	ID3D11Buffer* worldViewProj = this->worldMatrixBuffer.GetBuffer();

	context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	context->VSSetConstantBuffers(0, 1, &worldViewProj);

	this->subMeshes.at(0).PerformDrawCall(context);
	context->Draw(this->vertexBuffer.GetNrOfVertices(), 0);
}

