#include "headers/Scene.h"

Scene::~Scene()
{
	for (size_t i = 0; i < this->objectsInScene.size(); i++)
	{
		if (this->objectsInScene.at(i) != nullptr)
		{
			this->objectsInScene.at(i)->~MeshD3D11();
			this->objectsInScene.at(i) = nullptr;
		}
	}

	for (size_t i = 0; i < emitters.size(); i++)
	{
		delete emitters.at(i);
		emitters.at(i) = nullptr;
	}
}

void Scene::DrawScene(ID3D11DeviceContext*& immediateContext, ShaderD3D11 ps[2], bool shadowMapping)
//Draw all objects in the scene
{
	if (!shadowMapping)
	{
		for (size_t i = 0; i < this->objectsInScene.size(); i++)
		{
			ID3D11ShaderResourceView* srv;
			if (this->objectsInScene.at(i)->IsReflective())
			{
				//PSReflections
				ps[1].BindShader(immediateContext);
				srv = this->objectsInScene.at(i)->GetTextureCube()->GetCubeSRV();
				immediateContext->PSSetShaderResources(5, 1, &srv);
			}
			else //Regular Pixel Shader
				ps[0].BindShader(immediateContext);

			this->objectsInScene.at(i)->BindMeshBuffers(immediateContext);
			srv = nullptr;
			immediateContext->PSSetShaderResources(5, 1, &srv);
		}
	}
	else
		for (size_t i = 0; i < this->objectsInScene.size(); i++)
			this->objectsInScene.at(i)->BindMeshBuffers(immediateContext);

}

void Scene::AddObject(MeshD3D11*& object)
{
	this->objectsInScene.push_back(object);
}

int Scene::GetNrOfMeshes() const
{
	return this->objectsInScene.size();
}

MeshD3D11* Scene::GetMeshAt(int index) const
{
	return this->objectsInScene.at(index);
}

void Scene::AddLight(ID3D11Device* device, LightData data)
{
	if (data.directionalLight)
		this->directionalLights.InitializeLightSource(device, data);
	else
		this->spotLights.InitializeLightSource(device, data);
}

void Scene::UpdateNrOfLights(ID3D11Device* device)
{
	//Update buffer with number of lights for each light type
	NrOfLight NOL;
	NOL.nrOfSpotLights[0] = this->spotLights.GetNrOfLights();
	NOL.nrOfDirectionalLight[0] = this->directionalLights.GetNrOfLights();
	this->nrOfLightBuffer.Initialize(device, sizeof(NOL), &NOL);
}

UINT Scene::GetNrOfLights(LIGHT_TYPE type)
{
	if (type == SPOTLIGHT)
		return this->spotLights.GetNrOfLights();
	else
		return this->directionalLights.GetNrOfLights();
}

ID3D11Buffer* Scene::GetNrOfLightsBuffer()
{
	return this->nrOfLightBuffer.GetBuffer();
}

ID3D11DepthStencilView* Scene::GetShadowMapDSV(LIGHT_TYPE type, UINT lightIndex)
{
	if (type == SPOTLIGHT)
		return this->spotLights.GetShadowMapDSV(lightIndex);
	else
		return this->directionalLights.GetShadowMapDSV(lightIndex);
}

ID3D11ShaderResourceView* Scene::GetShadowMapsSRV(LIGHT_TYPE type)
{
	if (type == SPOTLIGHT)
		return this->spotLights.GetShadowMapsSRV();
	else
		return this->directionalLights.GetShadowMapsSRV();
}

ID3D11Buffer* Scene::GetLightCameraConstantBuffer(LIGHT_TYPE type, UINT lightIndex)
{
	if (type == SPOTLIGHT)
		return this->spotLights.GetLightCameraConstantBuffer(lightIndex);
	else
		return this->directionalLights.GetOrthConstantBuffer(lightIndex);
}

ID3D11ShaderResourceView* Scene::GetLightBufferSRV(LIGHT_TYPE type)
{
	if (type == SPOTLIGHT)
		return this->spotLights.GetLightBufferSRV();
	else
		return this->directionalLights.GetLightBufferSRV();
}

void Scene::InitializeStructuredBuffer(ID3D11Device* device)
{
	this->spotLights.InitializeStructuredBuffer(device);
	this->directionalLights.InitializeStructuredBuffer(device);
}

void Scene::InitializeParticleEmitter(ID3D11Device* device, UINT nrOfParticles, DirectX::XMFLOAT3 position,float maxAngle, float maxDistance, float speed, PARTICLE_TYPE type)
{
	Emitter* emitter = new Emitter();
	emitter->Initialize(device, nrOfParticles, position, maxAngle, maxDistance, speed, type);
	this->emitters.push_back(emitter);
}

UINT Scene::GetEmitterCount() const
{
	return this->emitters.size();
}

Emitter* Scene::GetEmitterAt(UINT index) const
{
	return this->emitters.at(index);
}