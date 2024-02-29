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

	XMFLOAT4X4 world[2];
	XMStoreFloat4x4(&world[0], XMMatrixTranspose(XMMatrixTranslation(worldX, worldY, worldZ)));
	XMMATRIX view = XMMatrixLookAtLH({ 0, 0, -10 }, { 0, 0, 0 }, { 0, 1, 0 });
	XMMATRIX proj = XMMatrixPerspectiveFovLH(1, 16.0f / 9, 1, 20);
	XMStoreFloat4x4(&world[1], XMMatrixTranspose(XMMatrixMultiply(view, proj)));
	this->worldMatrixBuffer.Initialize(device, sizeof(XMFLOAT4X4) * 2, &world);
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

