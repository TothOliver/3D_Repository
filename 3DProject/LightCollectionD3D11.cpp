#include "headers/LightCollectionD3D11.h"

LightCollectionD3D11::~LightCollectionD3D11()
{
	for (int i = 0; i < shadowCameras.size(); i++) {
		delete shadowCameras[i];
		shadowCameras[i] = nullptr;
	}
}

void LightCollectionD3D11::InitializeLightSource(ID3D11Device* device, LightData& lightInfo)
{
	CameraD3D11* lightCamera = new CameraD3D11();
	LightBuffer light;
	
	lightCamera->Initialize(device, { lightInfo.fovAngleY, lightInfo.aspectRatio, lightInfo.nearZ, lightInfo.farZ }, 
		{ lightInfo.initialPosition.x, lightInfo.initialPosition.y ,lightInfo.initialPosition.z });
	lightCamera->RotateRight(lightInfo.rotationY);
	lightCamera->RotateUp(lightInfo.rotationX);

	ID3D11DeviceContext* immediateContext;
	device->GetImmediateContext(&immediateContext);
	lightCamera->UpdateInternalConstantBuffer(immediateContext);


	if (lightInfo.directionalLight)
		light.vpMatrix = lightCamera->GetOrthMatrix({ lightInfo.fovAngleY, lightInfo.aspectRatio, lightInfo.nearZ, lightInfo.farZ }); //new
	else
		light.vpMatrix = lightCamera->GetViewProjectionMatrix();


	light.angle = lightInfo.angle;
	light.colour = lightInfo.colour;
	light.direction = lightCamera->GetForward();
	light.position = lightInfo.initialPosition;
	light.intensity = lightInfo.intensity;

	bufferData.push_back(light);
	shadowCameras.push_back(lightCamera);

	immediateContext->Release();
}

void LightCollectionD3D11::InitializeStructuredBuffer(ID3D11Device* device)
{
	if (GetNrOfLights() > 0)
	{
		this->shadowMaps.Initialize(device, 1024, 576, false, GetNrOfLights());
		this->lightBuffer.Initialize(device, sizeof(LightBuffer), bufferData.size(), bufferData.data(), true);
	}
}

void LightCollectionD3D11::UpdateLightBuffers(ID3D11DeviceContext* context)
{

	for (size_t i = 0; i < bufferData.size(); ++i)
	{
		bufferData[i].vpMatrix = shadowCameras[i]->GetViewProjectionMatrix();
		this->lightBuffer.UpdateBuffer(context, &bufferData[i]);
	}
}

UINT LightCollectionD3D11::GetNrOfLights() const
{
	return bufferData.size();
}

ID3D11DepthStencilView* LightCollectionD3D11::GetShadowMapDSV(UINT lightIndex) const
{
	return this->shadowMaps.GetDSV(lightIndex);
}

ID3D11ShaderResourceView* LightCollectionD3D11::GetShadowMapsSRV() const
{
	return this->shadowMaps.GetSRV();
}

ID3D11ShaderResourceView* LightCollectionD3D11::GetLightBufferSRV() const
{
	return this->lightBuffer.GetSRV();
}

ID3D11Buffer* LightCollectionD3D11::GetLightBuffer() const
{
	return this->lightBuffer.GetBuffer();
}

ID3D11Buffer* LightCollectionD3D11::GetLightCameraConstantBuffer(UINT lightIndex) const
{
	return this->shadowCameras.at(lightIndex)->GetVPBuffer();
}

ID3D11Buffer* LightCollectionD3D11::GetOrthConstantBuffer(UINT lightIndex) const
{
	return this->shadowCameras.at(lightIndex)->GetOrthBuffer();
}


