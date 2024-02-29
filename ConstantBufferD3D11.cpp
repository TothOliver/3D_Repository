#include "headers/ConstantBufferD3D11.h"
#include <DirectXMath.h>

ConstantBufferD3D11::ConstantBufferD3D11(ID3D11Device* device, size_t byteSize, void* initialData)
{
	Initialize(device, byteSize, initialData);
}

ConstantBufferD3D11::~ConstantBufferD3D11()
{
	//this->buffer->Release();
}

ConstantBufferD3D11::ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept
{
	this->buffer = other.buffer;
	this->bufferSize = other.bufferSize;
	this->mappedResource = other.mappedResource;
}

void ConstantBufferD3D11::Initialize(ID3D11Device* device, size_t byteSize, void* initialData)
{
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	mappedResource.RowPitch = sizeof(DirectX::XMFLOAT4X4);
	
	this->bufferSize = byteSize;

	D3D11_BUFFER_DESC constBufferDesc;
	constBufferDesc.ByteWidth = byteSize;
	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cameraData;
	cameraData.pSysMem = initialData;
	cameraData.SysMemPitch = 0;
	cameraData.SysMemSlicePitch = 0;

	device->CreateBuffer(&constBufferDesc, &cameraData, &this->buffer);
}

size_t ConstantBufferD3D11::GetSize() const
{
	return this->bufferSize;
}

ID3D11Buffer* ConstantBufferD3D11::GetBuffer() const
{
	return this->buffer;
}

void ConstantBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	context->Map(this->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &this->mappedResource);
	memcpy(this->mappedResource.pData, data, this->bufferSize);
	context->Unmap(this->buffer, 0);
}
