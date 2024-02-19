#pragma once

#include "Parser.h"
#include "MeshD3D11.h"

MeshD3D11::MeshD3D11(ID3D11Device*& device, std::string filename, float origin[3])
{
	MeshData mData;
	ParseObj(device, filename, mData.vertexData, mData.indexData, this->subMeshes);

	this->vertexBuffer.Initialize(device, 8, mData.vertexData.size(), mData.vertexData.data());

	this->indexBuffer.Initialize(device, mData.indexData.size(), mData.indexData.data());
}