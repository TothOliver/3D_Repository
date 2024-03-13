#pragma once

#include "headers/IndexBufferD3D11.h"

IndexBufferD3D11::IndexBufferD3D11(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData)
{
	Initialize(device, nrOfIndicesInBuffer, indexData);
}

IndexBufferD3D11::~IndexBufferD3D11()
{
	this->buffer->Release();
}

void IndexBufferD3D11::Initialize(ID3D11Device* device, size_t nrOfIndicesInBuffer, uint32_t* indexData)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = nrOfIndicesInBuffer * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = indexData;

	device->CreateBuffer(&bufferDesc, &subData, &this->buffer);
}