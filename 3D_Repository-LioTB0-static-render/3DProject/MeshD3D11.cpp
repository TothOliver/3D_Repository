#pragma once

#include "Parser.h"
#include "MeshD3D11.h"

using namespace DirectX;

MeshD3D11::MeshD3D11(ID3D11Device*& device, std::string filename, float worldX, float worldY, float worldZ)
{
	MeshData mData;
	ParseObj(device, filename, mData.vertexData, mData.indexData, this->subMeshes);

	this->vertexBuffer.Initialize(device, 8, mData.vertexData.size(), mData.vertexData.data());

	this->indexBuffer.Initialize(device, mData.indexData.size(), mData.indexData.data());

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixTranspose(XMMatrixTranslation(worldX, worldY, worldZ)));
	this->worldMatrixBuffer.Initialize(device, sizeof(XMFLOAT4X4), &world);
}

