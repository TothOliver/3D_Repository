#pragma once

#include "ShaderD3D11.h"
#include "MeshD3D11.h"
#include "LightCollectionD3D11.h"

class Scene
{
private:
	std::vector<MeshD3D11*> objectsInScene;

	LightCollectionD3D11 spotLights;
	LightCollectionD3D11 directionalLights;
	ConstantBufferD3D11 nrOfLightBuffer;

public:
	Scene() = default;
	~Scene();
	void DrawScene(ID3D11DeviceContext*& immediateContext, ShaderD3D11 ps[2], bool shadowMapping);
	void AddObject(MeshD3D11*& object);

	int GetNrOfMeshes() const;
	MeshD3D11* GetMeshAt(int index) const;

	void AddLight(ID3D11Device* device, LightData data);

	void UpdateNrOfLights(ID3D11Device* device);

	UINT GetNrOfLights(LIGHT_TYPE type);

	ID3D11Buffer* GetNrOfLightsBuffer();

	ID3D11DepthStencilView* GetShadowMapDSV(LIGHT_TYPE type, UINT lightIndex);

	ID3D11ShaderResourceView* GetShadowMapsSRV(LIGHT_TYPE type);

	ID3D11Buffer* GetLightCameraConstantBuffer(LIGHT_TYPE type, UINT lightIndex);

	ID3D11ShaderResourceView* GetLightBufferSRV(LIGHT_TYPE type);

	void InitializeStructuredBuffer(ID3D11Device* device);
};