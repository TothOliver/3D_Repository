#pragma once

#include <vector>

#include <d3d11_4.h>

#include "SubMeshD3D11.h"
#include "VertexBufferD3D11.h"
#include "IndexBufferD3D11.h"
#include <vector>
#include "ConstantBufferD3D11.h"

struct MeshData
{
	std::vector<float> vertexData;
	std::vector<uint32_t> indexData;
	std::vector<Material> materials;

	float worldX = 0;
	float worldY = 0;
	float worldZ = 0;

};

class MeshD3D11
{
private:
	MeshData mData;
	std::vector<SubMeshD3D11*> subMeshes;
	VertexBufferD3D11 vertexBuffer;
	IndexBufferD3D11 indexBuffer;
	ConstantBufferD3D11 worldMatrixBuffer;

public:
	MeshD3D11() = default;
	MeshD3D11(ID3D11Device* &device, std::string filename, float worldX = 0, float worldY = 0, float worldZ = 0, float scale = 1);
	~MeshD3D11();
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