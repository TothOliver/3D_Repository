#pragma once

#include "VertexBufferD3D11.h"


void VertexBufferD3D11::Initialize(ID3D11Device* device, UINT sizeOfVertex,
	UINT nrOfVerticesInBuffer, void* vertexData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeOfVertex * nrOfVerticesInBuffer * sizeof(float);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = vertexData;

	device->CreateBuffer(&bufferDesc, &subData, &this->buffer);
}

VertexBufferD3D11::VertexBufferD3D11(ID3D11Device* device, UINT sizeOfVertex,
	UINT nrOfVerticesInBuffer, void* vertexData)
{
	this->Initialize(device, sizeOfVertex, nrOfVerticesInBuffer, vertexData);
}

VertexBufferD3D11::~VertexBufferD3D11()
{
	buffer->Release();
}

UINT VertexBufferD3D11::GetNrOfVertices() const
{
	return this->nrOfVertices;
}

UINT VertexBufferD3D11::GetVertexSize() const
{
	return this->vertexSize;
}

ID3D11Buffer* VertexBufferD3D11::GetBuffer() const
{
	return this->buffer;
}