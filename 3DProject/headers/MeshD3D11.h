#pragma once

#include <vector>

#include <d3d11_4.h>
#include <DirectXCollision.h>

#include "SubMeshD3D11.h"
#include "VertexBufferD3D11.h"
#include <vector>
#include "ConstantBufferD3D11.h"
#include "TextureCube.h"
#include "ShaderResourceD3D11.h"

struct MeshData
{
	std::vector<float> vertexData;
	std::vector<uint32_t> indexData;
	std::vector<Material> materials;

	float worldX = 0;
	float worldY = 0;
	float worldZ = 0;

};

struct BoundingBoxPositions {
	DirectX::XMVECTOR pos1;
	DirectX::XMVECTOR pos2;
};

class MeshD3D11
{
private:
	MeshData mData;
	std::vector<SubMeshD3D11*> subMeshes;
	VertexBufferD3D11 vertexBuffer;
	ConstantBufferD3D11 worldMatrixBuffer;
	ConstantBufferD3D11 meshCenterPosition;

	DirectX::XMFLOAT4X4 world;
	BoundingBoxPositions bbp;
	DirectX::BoundingBox bb;

	ShaderResourceD3D11 displacementMap;

	TextureCube* texCube;
	bool reflective;
	bool displace;

public:
	MeshD3D11() = default;
	MeshD3D11(ID3D11Device* device, std::string filename, float worldX = 0, float worldY = 0, float worldZ = 0, float scale = 1, bool displace = false, bool reflective = false, UINT reflectionDetail = 512, UINT radius = 0);
	~MeshD3D11();
	MeshD3D11(const MeshD3D11 & other) = delete;
	MeshD3D11& operator=(const MeshD3D11 & other) = delete;
	MeshD3D11(MeshD3D11 && other) = delete;
	MeshD3D11& operator=(MeshD3D11 && other) = delete;

	void BindMeshBuffers(ID3D11DeviceContext* context) const;

	size_t GetNrOfSubMeshes() const;
	ID3D11ShaderResourceView* GetAmbientSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetDiffuseSRV(size_t subMeshIndex) const;
	ID3D11ShaderResourceView* GetSpecularSRV(size_t subMeshIndex) const;
	bool IsReflective() const;
	bool IsDisplaced() const;
	TextureCube* GetTextureCube() const;

	DirectX::XMFLOAT3 GetWorldPosition();
	DirectX::XMVECTOR GetBoundingBox1();
	DirectX::XMVECTOR GetBoundingBox2();

	DirectX::BoundingBox GetBoundingBox() const;
	DirectX::XMMATRIX GetWorldMatrix() const;

	ID3D11Buffer* GetMeshCenterBuffer() const;
};

DirectX::XMFLOAT3 CalculateBoundingBoxPointsMin(float pX, float pY, float pZ, float& minX, float& minY, float& minZ);
DirectX::XMFLOAT3 CalculateBoundingBoxPointsMax(float pX, float pY, float pZ, float& maxX, float& maxY, float& maxZ);