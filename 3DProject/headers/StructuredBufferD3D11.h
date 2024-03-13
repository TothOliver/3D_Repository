#pragma once

#include <d3d11_4.h>

class StructuredBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	UINT elementSize = 0;
	size_t nrOfElements = 0;

public:
	StructuredBufferD3D11() = default;
	StructuredBufferD3D11(ID3D11Device* device, UINT sizeOfElement,
		size_t nrOfElementsInBuffer, void* bufferData = nullptr, bool dynamic = true);
	~StructuredBufferD3D11();
	StructuredBufferD3D11(const StructuredBufferD3D11& other) = delete;
	StructuredBufferD3D11& operator=(const StructuredBufferD3D11& other) = delete;
	StructuredBufferD3D11(StructuredBufferD3D11&& other) = delete;
	StructuredBufferD3D11 operator=(StructuredBufferD3D11&& other) = delete;

	void Initialize(ID3D11Device* device, UINT sizeOfElement,
		size_t nrOfElementsInBuffer, void* bufferData = nullptr, bool dynamic = true);

	void UpdateBuffer(ID3D11DeviceContext* context, void* data);

	UINT GetElementSize() const;
	size_t GetNrOfElements() const;
	ID3D11ShaderResourceView* GetSRV() const;
};