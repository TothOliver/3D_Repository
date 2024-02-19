#pragma once

#include <d3d11_4.h>

class VertexBufferD3D11
{
private:
	ID3D11Buffer* buffer = nullptr;
	UINT nrOfVertices = 0;
	UINT vertexSize = 0;

public:
	VertexBufferD3D11() = default;
	void Initialize(ID3D11Device* device, UINT sizeOfVertex,
		UINT nrOfVerticesInBuffer, void* vertexData);

	VertexBufferD3D11(ID3D11Device* device, UINT sizeOfVertex, 
		UINT nrOfVerticesInBuffer, void* vertexData);
	~VertexBufferD3D11();

	VertexBufferD3D11(const VertexBufferD3D11& other) = delete;
	VertexBufferD3D11& operator=(const VertexBufferD3D11& other) = delete;
	VertexBufferD3D11(VertexBufferD3D11&& other) = delete;
	VertexBufferD3D11& operator=(VertexBufferD3D11&& other) = delete;
	
	UINT GetNrOfVertices() const;
	UINT GetVertexSize() const;
	ID3D11Buffer* GetBuffer() const;
};