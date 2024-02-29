#pragma once

#include <vector>

#include <d3d11_4.h>
#include <DirectXMath.h>

#include "StructuredBufferD3D11.h"
#include "DepthBufferD3D11.h"
#include "CameraD3D11.h"

struct SpotLightData
{
	struct ShadowMapInfo
	{
		UINT textureDimension = 0;
	} shadowMapInfo;

	struct PerLightInfo
	{
		DirectX::XMFLOAT3 colour;
		float rotationX = 0.0f;
		float rotationY = 0.0f;
		float angle = 0.0f;
		float projectionNearZ = 0.0f;
		float projectionFarZ = 0.0f;
		DirectX::XMFLOAT3 initialPosition;
	};

	std::vector<PerLightInfo> perLightInfo;
};

class SpotLightCollectionD3D11
{
private:
	struct LightBuffer
	{
		DirectX::XMFLOAT4X4 vpMatrix;
		DirectX::XMFLOAT3 colour;
		DirectX::XMFLOAT3 direction;
		float angle = 0.0f;
		DirectX::XMFLOAT3 position;
	};

	std::vector<LightBuffer> bufferData;

	DepthBufferD3D11 shadowMaps;
	StructuredBufferD3D11 lightBuffer;
	std::vector<CameraD3D11> shadowCameras;

public:
	SpotLightCollectionD3D11() = default;
	~SpotLightCollectionD3D11() = default;
	SpotLightCollectionD3D11(const SpotLightCollectionD3D11& other) = delete;
	SpotLightCollectionD3D11& operator=(const SpotLightCollectionD3D11& other) = delete;
	SpotLightCollectionD3D11(SpotLightCollectionD3D11&& other) = delete;
	SpotLightCollectionD3D11& operator=(DepthBufferD3D11 && other) = delete;

	void Initialize(ID3D11Device* device, const SpotLightData& lightInfo);

	void UpdateLightBuffers(ID3D11DeviceContext* context);

	UINT GetNrOfLights() const;
	ID3D11DepthStencilView* GetShadowMapDSV(UINT lightIndex) const;
	ID3D11ShaderResourceView* GetShadowMapsSRV() const;
	ID3D11ShaderResourceView* GetLightBufferSRV() const;
	ID3D11Buffer* GetLightCameraConstantBuffer(UINT lightIndex) const;
};