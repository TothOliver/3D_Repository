#include "headers/SpotLightCollectionD3D11.h"


SpotLightCollectionD3D11::~SpotLightCollectionD3D11()
{
	for (int i = 0; i < shadowCameras.size(); i++) {
		shadowCameras[i]->~CameraD3D11();
	}
}

void SpotLightCollectionD3D11::InitializeLightSource(ID3D11Device* device, SpotLightData& lightInfo)
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

	if (lightInfo.directionalLight) {
		DirectX::XMMATRIX orthoProjectionMatrix = DirectX::XMMatrixOrthographicLH(100, 100, lightInfo.nearZ, lightInfo.farZ);
		XMStoreFloat4x4(&light.vpMatrix, orthoProjectionMatrix);
	}
	else {
		light.vpMatrix = lightCamera->GetViewProjectionMatrix();
	}

	light.angle = lightInfo.angle;
	light.colour = lightInfo.colour;
	light.direction = lightCamera->GetForward();
	light.position = lightInfo.initialPosition;
	light.intensity = lightInfo.intensity;

	bufferData.push_back(light);
	shadowCameras.push_back(lightCamera);

	immediateContext->Release();
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
		bufferData[i].vpMatrix = shadowCameras[i]->GetViewProjectionMatrix();
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
	return this->shadowCameras.at(lightIndex)->GetVPBuffer();
}  


