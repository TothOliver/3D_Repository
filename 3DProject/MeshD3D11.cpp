#pragma once
#include "headers/Parser.h"
#include "headers/MeshD3D11.h"

using namespace DirectX;

MeshD3D11::MeshD3D11(ID3D11Device* device, std::string filename, float worldX, float worldY, float worldZ, float scale, bool displace, bool reflective, UINT reflectionDetail, UINT radius)
{
	this->displace = displace;
	this->reflective = reflective;
	this->texCube = new TextureCube();
	
	if (reflective)
	{
		this->texCube->Initialize(device, reflectionDetail, radius, {worldX, worldY, worldZ});
	}

	mData.worldX = worldX;
	mData.worldY = worldY;
	mData.worldZ = worldZ;

	ParseObj(device, filename, mData.vertexData, mData.indexData, this->subMeshes, this->mData.materials, this->bbp);

	for (size_t i = 0; i < this->subMeshes.size(); i++)
	{
		this->subMeshes.at(i)->Initialize(device);
	}

	if (this->displace) 
	{
		std::string path = std::string("assets/") + filename.substr(0, filename.length() - 4) + std::string("Displacement.jpg");
		this->displacementMap.Initialize(device, path.c_str());
	}


	this->vertexBuffer.Initialize(device, 11, mData.vertexData.size() / 11, mData.vertexData.data());

	XMStoreFloat4x4(&this->world, XMMatrixTranspose(XMMatrixMultiply(XMMatrixScaling(scale, scale, scale), XMMatrixTranslation(worldX, worldY, worldZ))));
	this->worldMatrixBuffer.Initialize(device, sizeof(XMFLOAT4X4), &this->world);

	XMFLOAT4 position = XMFLOAT4(worldX, worldY, worldZ, 1);
	this->meshCenterPosition.Initialize(device, sizeof(XMFLOAT4), &position);
}

MeshD3D11::~MeshD3D11()
{
	delete texCube;
	texCube = nullptr;

	for (size_t i = 0; i < subMeshes.size(); i++)
	{
		delete subMeshes.at(i);
		subMeshes.at(i) = nullptr;
	}

	for (size_t i = 0; i < this->mData.materials.size(); i++)
	{
		this->mData.materials.at(i).ambientTexture->Release();
		this->mData.materials.at(i).diffuseTexture->Release();
		this->mData.materials.at(i).specularTexture->Release();
		this->mData.materials.at(i).normalTexture->Release();
		this->mData.materials.at(i).heightTexture->Release();
	}

}

void MeshD3D11::BindMeshBuffers(ID3D11DeviceContext* context) const
{
	ID3D11Buffer* vBuffer = this->vertexBuffer.GetBuffer();
	UINT stride = this->vertexBuffer.GetVertexSize() * sizeof(float);
	UINT offset = 0;
	ID3D11Buffer* worldViewProj = this->worldMatrixBuffer.GetBuffer();

	context->IASetVertexBuffers(0, 1, &vBuffer, &stride, &offset);
	context->VSSetConstantBuffers(0, 1, &worldViewProj);

	if (displace)
	{
		ID3D11ShaderResourceView* disp = this->displacementMap.GetSRV();
		context->DSSetShaderResources(0, 1, &disp);
	}

	for (size_t i = 0; i < subMeshes.size(); i++)
	{
		this->subMeshes.at(i)->PerformDrawCall(context, this->displace);
	}
}

bool MeshD3D11::IsReflective() const
{
	return this->reflective;
}

bool MeshD3D11::IsDisplaced() const
{
	return this->displace;
}

TextureCube* MeshD3D11::GetTextureCube() const
{
	return this->texCube;
}

DirectX::XMFLOAT3 MeshD3D11::GetWorldPosition()
{
	return { this->mData.worldX, this->mData.worldY, this->mData.worldZ };
}

DirectX::XMVECTOR MeshD3D11::GetBoundingBox1()
{
	return this->bbp.pos1;
}

DirectX::XMVECTOR MeshD3D11::GetBoundingBox2()
{
	return this->bbp.pos2;
}

DirectX::BoundingBox MeshD3D11::GetBoundingBox() const
{
	DirectX::BoundingBox transformed = this->bb;
	transformed.Transform(transformed, XMMatrixTranspose(this->GetWorldMatrix()));
	return transformed;
}

DirectX::XMMATRIX MeshD3D11::GetWorldMatrix() const
{
	return XMLoadFloat4x4(&this->world);
}

ID3D11Buffer* MeshD3D11::GetMeshCenterBuffer() const
{
	return this->meshCenterPosition.GetBuffer();
}

DirectX::XMFLOAT3 CalculateBoundingBoxPointsMin(float pX, float pY, float pZ, float& minX, float& minY, float& minZ)
{
	if (pX < minX || minX == NULL)
		minX = pX;
	if (pY < minY || minY == NULL)
		minY = pY;
	if (pZ < minZ || minZ == NULL)
		minZ = pZ;

	return { minX, minY, minZ };;
}

DirectX::XMFLOAT3 CalculateBoundingBoxPointsMax(float pX, float pY, float pZ, float& maxX, float& maxY, float& maxZ)
{
	if (pX > maxX || maxX == NULL)
		maxX = pX;
	if (pY > maxY || maxY == NULL)
		maxY = pY;
	if (pZ > maxZ || maxZ == NULL)
		maxZ = pZ;

	return { maxX, maxY, maxZ };
}