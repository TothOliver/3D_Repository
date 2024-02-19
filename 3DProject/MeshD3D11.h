#pragma once

#include <vector>

#include <d3d11_4.h>

#include "SubMeshD3D11.h"
#include "VertexBufferD3D11.h"
#include "IndexBufferD3D11.h"
#include <vector>

struct MeshData
{
	std::vector<float> vertexData;
	std::vector<uint32_t> indexData;
	float origin[3];

	struct SubMeshInfo
	{
		size_t startIndexValue;
		size_t nrOfIndicesInSubMesh;
		Material mat;
	};

	std::vector<SubMeshInfo> subMeshInfo;
};

class MeshD3D11
{
private:
	std::vector<SubMeshD3D11> subMeshes;
	VertexBufferD3D11 vertexBuffer;
	IndexBufferD3D11 indexBuffer;

public:
	MeshD3D11() = default;
	MeshD3D11(ID3D11Device* &device, std::string filename, float origin[3]);
	~MeshD3D11() = default;
	MeshD3D11(const MeshD3D11 & other) = delete;
	MeshD3D11& operator=(const MeshD3D11 & other) = delete;
	MeshD3D11(MeshD3D11 && other) = delete;
	MeshD3D11& operator=(MeshD3D11 && other) = delete;

	void Initialize(ID3D11Device* device, const MeshData& meshInfo);

	void BindMeshBuffers(ID3D11DeviceContext* context) const;
	void PerformSubMeshDrawCall(ID3D11DeviceContext* context, size_t subMeshIndex) const;

	size_t GetNrOfSubMeshes() const;
	ID3D11ShaderResourceView* GetAmbientSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetDiffuseSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetSpecularSRV(size_t subMeshIndex) const;
};