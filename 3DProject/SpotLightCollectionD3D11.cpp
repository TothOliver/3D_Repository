#include "headers/SpotLightCollectionD3D11.h"


void SpotLightCollectionD3D11::InitializeLightSource(ID3D11Device* device, SpotLightData& lightInfo)
{
	CameraD3D11 lightCamera;
	LightBuffer light;
	
	lightCamera.Initialize(device, { 1, (float)16 / 9, 0.05, 100 }, { lightInfo.pli.initialPosition.x, lightInfo.pli.initialPosition.y ,lightInfo.pli.initialPosition.z });

	light.vpMatrix = lightCamera.GetViewProjectionMatrix();
	light.angle = lightInfo.pli.angle;
	light.colour = lightInfo.pli.colour;
	light.direction = lightInfo.pli.direction;
	light.position = lightInfo.pli.initialPosition;
	light.intensity = lightInfo.pli.intensity;

	bufferData.push_back(light);
	shadowCameras.push_back(lightCamera);
}

void SpotLightCollectionD3D11::InitializeStructuredBuffer(ID3D11Device* device)
{
	this->shadowMaps.Initialize(device, 1024, 576, false, GetNrOfLights());
	this->lightBuffer.Initialize(device, sizeof(LightBuffer), bufferData.size(), bufferData.data(), true);
}

void SpotLightCollectionD3D11::UpdateLightBuffers(ID3D11DeviceContext* context)
{

	for (size_t i = 0; i < bufferData.size(); ++i)
	{
		bufferData[i].vpMatrix = shadowCameras[i].GetViewProjectionMatrix();
		this->lightBuffer.UpdateBuffer(context, &bufferData[i]);
	}
}

UINT SpotLightCollectionD3D11::GetNrOfLights() const
{
	return bufferData.size();
}

ID3D11DepthStencilView* SpotLightCollectionD3D11::GetShadowMapDSV(UINT lightIndex) const
{
	return this->shadowMaps.GetDSV(lightIndex);
}

ID3D11ShaderResourceView* SpotLightCollectionD3D11::GetShadowMapsSRV() const
{
	return this->shadowMaps.GetSRV();
}

ID3D11ShaderResourceView* SpotLightCollectionD3D11::GetLightBufferSRV() const
{
	return this->lightBuffer.GetSRV();
}

ID3D11Buffer* SpotLightCollectionD3D11::GetLightBuffer() const
{
	return this->lightBuffer.GetBuffer();
}

ID3D11Buffer* SpotLightCollectionD3D11::GetLightCameraConstantBuffer(UINT lightIndex) const
{
	return this->shadowCameras[lightIndex].GetVPBuffer();
}  

void CreatePerLightInfo(SpotLightData& light, DirectX::XMFLOAT4 colour, DirectX::XMFLOAT3 direction, float angle, DirectX::XMFLOAT4 initialPosition, DirectX::XMFLOAT4 intensity)
{
	light.pli.colour = colour;
	light.pli.direction = direction;
	light.pli.angle = angle;
	light.pli.initialPosition = initialPosition;
	light.pli.intensity = intensity;
}
