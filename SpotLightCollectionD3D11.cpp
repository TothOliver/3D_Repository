#include "SpotLightCollectionD3D11.h"

void SpotLightCollectionD3D11::InitializeLightSource(ID3D11Device* device, const SpotLightData& lightInfo)
{
	LightBuffer light;
	static size_t lightIndex = 0;

	//light.vpMatrix = vpMatrix;
	light.angle = lightInfo.perLightInfo[lightIndex].angle;
	light.colour = lightInfo.perLightInfo[lightIndex].colour;
	//light.direction = direction;
	light.position = lightInfo.perLightInfo[lightIndex].initialPosition;
	light.intensity = lightInfo.perLightInfo[lightIndex].intensity;

	bufferData.push_back(light);
	lightIndex++;
}

void SpotLightCollectionD3D11::InitializeStructuredBuffer(ID3D11Device* device)
{
	this->lightBuffer.Initialize(device, sizeof(LightBuffer), bufferData.size(), &bufferData, true);
}

void SpotLightCollectionD3D11::UpdateLightBuffers(ID3D11DeviceContext* context)
{
	this->lightBuffer.UpdateBuffer(context, &this->lightBuffer); //?
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

ID3D11Buffer* SpotLightCollectionD3D11::GetLightCameraConstantBuffer(UINT lightIndex) const
{
	return nullptr;
}
