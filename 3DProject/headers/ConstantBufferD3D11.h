#pragma once

#include <iostream>
#include <d3d11_4.h>
#include <DirectXMath.h>

class ConstantBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	size_t bufferSize = 0;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

public:
	ConstantBufferD3D11() = default;
	ConstantBufferD3D11(ID3D11Device* device, size_t byteSize, void* initialData);
	~ConstantBufferD3D11();

	ConstantBufferD3D11(const ConstantBufferD3D11& other) = delete;
	ConstantBufferD3D11& operator=(const ConstantBufferD3D11& other) = delete;
	ConstantBufferD3D11(ConstantBufferD3D11&& other) noexcept; // Move constructor
	ConstantBufferD3D11& operator=(ConstantBufferD3D11&& other) noexcept; // Move assignment operator

	void Initialize(ID3D11Device* device, size_t byteSize, void* initialData = nullptr);

	size_t GetSize() const;
	ID3D11Buffer* GetBuffer() const;
	void UpdateBuffer(ID3D11DeviceContext* context, void* data);
};