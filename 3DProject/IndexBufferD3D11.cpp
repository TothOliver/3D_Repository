#pragma once

#include "headers/IndexBufferD3D11.h"
#include <vector>
#include <numeric>

IndexBufferD3D11::~IndexBufferD3D11()
{
	this->buffer->Release();
}

void IndexBufferD3D11::Initialize(ID3D11Device* device, size_t nrOfIndicesInBuffer, size_t startIndex)
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = nrOfIndicesInBuffer * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	std::vector<uint32_t> indexSlice(nrOfIndicesInBuffer);
	std::iota(std::begin(indexSlice), std::end(indexSlice), startIndex);

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = indexSlice.data();

	device->CreateBuffer(&bufferDesc, &subData, &this->buffer);
}

ID3D11Buffer* IndexBufferD3D11::GetBuffer() const
{
	return this->buffer;
}