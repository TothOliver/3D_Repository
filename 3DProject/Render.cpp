#pragma once
#include <vector>
#include "headers/InputLayoutD3D11.h"
#include "headers/SamplerD3D11.h"
#include "headers/Render.h"
#include "headers/ShaderD3D11.h"
#include "headers/SceneManager.h"
#include "headers/TextureCube.h"
#include "headers/GBuffer.h"

void RenderToTargetUAV(ID3D11Device* device, ID3D11DeviceContext* immediateContext, ID3D11UnorderedAccessView* uav,
	const UINT WIDTH, const UINT HEIGHT, ID3D11DepthStencilView* dsView, ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampler,
	D3D11_VIEWPORT viewport, ID3D11Buffer* vpBuffer, ID3D11Buffer* cameraPosition, Scene* scene, int uavSlot,
	ShaderD3D11& vs, ShaderD3D11& cs, ShaderD3D11 ps[2], ShaderD3D11 particleShaders[4])
{
	ID3D11RenderTargetView* rtv[3];
	ID3D11ShaderResourceView* srv[3];

	GBuffer positionBuffer(device, WIDTH, HEIGHT);
	GBuffer normalBuffer(device, WIDTH, HEIGHT);
	GBuffer diffuseBuffer(device, WIDTH, HEIGHT);

	rtv[0] = positionBuffer.GetRTV();
	rtv[1] = normalBuffer.GetRTV();
	rtv[2] = diffuseBuffer.GetRTV();

	srv[0] = positionBuffer.GetSRV();
	srv[1] = normalBuffer.GetSRV();
	srv[2] = diffuseBuffer.GetSRV();

	ID3D11RenderTargetView* RTVnull[3];
	ID3D11ShaderResourceView* SRVnull[7];;
	ID3D11UnorderedAccessView* UAVnull = nullptr;

	float clear[4] = { 0, 0, 0, 1 };

	for (size_t i = 0; i < 3; i++)
	{
		RTVnull[i] = nullptr;
		immediateContext->ClearRenderTargetView(rtv[i], clear);
	}
	for (size_t i = 0; i < 7; i++)
	{
		SRVnull[i] = nullptr;
	}

	immediateContext->ClearUnorderedAccessViewFloat(uav, clear);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	//Same sampler is used for both particles pass and main pass
	immediateContext->PSSetSamplers(0, 1, &sampler);

	//Render particles for each emitter in scenee
	for (size_t i = 0; i < scene->GetEmitterCount(); i++)
	{
		Emitter* currentEmitter = scene->GetEmitterAt(i);

		//Draw particles
		ID3D11ShaderResourceView* particleSRV = currentEmitter->GetBufferSRV();
		ID3D11ShaderResourceView* particleTexture = currentEmitter->GetTextureSRV();

		immediateContext->IASetInputLayout(nullptr);
		immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		particleShaders[1].BindShader(immediateContext);

		immediateContext->VSSetShaderResources(0, 1, &particleSRV);
		particleShaders[2].BindShader(immediateContext);
		immediateContext->GSSetConstantBuffers(0, 1, &cameraPosition);
		immediateContext->GSSetConstantBuffers(1, 1, &vpBuffer);

		immediateContext->RSSetViewports(1, &viewport);
		immediateContext->PSSetShaderResources(0, 1, &particleTexture);
		particleShaders[3].BindShader(immediateContext);

		immediateContext->OMSetRenderTargets(3, rtv, dsView);
		immediateContext->Draw(currentEmitter->GetNrOfParticles(), 0);

		immediateContext->GSSetShader(nullptr, nullptr, 0);
		ID3D11ShaderResourceView* nullSRV = nullptr;
		immediateContext->VSSetShaderResources(0, 1, &nullSRV);
	}

	//Bind the shaders for the main render
	vs.BindShader(immediateContext);
	cs.BindShader(immediateContext);

	immediateContext->VSSetConstantBuffers(1, 1, &vpBuffer);

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	immediateContext->PSSetConstantBuffers(0, 1, &cameraPosition);

	immediateContext->RSSetViewports(1, &viewport);

	immediateContext->OMSetRenderTargets(3, rtv, dsView);
	scene->DrawScene(immediateContext, ps, false);
	immediateContext->OMSetRenderTargets(3, RTVnull, nullptr);


	immediateContext->CSSetConstantBuffers(0, 1, &cameraPosition);

	//Window unordered access view
	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &uav, nullptr);

	//GBuffers shader resource view
	immediateContext->CSSetShaderResources(0, 3, srv);

	//For shadow mapping
	ID3D11ShaderResourceView* srvSpotlightMaps = scene->GetShadowMapsSRV(SPOTLIGHT);
	ID3D11ShaderResourceView* srvDirectionalMaps = scene->GetShadowMapsSRV(DIRECTIONAL);
	ID3D11ShaderResourceView* srvSpotlight = scene->GetLightBufferSRV(SPOTLIGHT);
	ID3D11ShaderResourceView* srvDirectional = scene->GetLightBufferSRV(DIRECTIONAL);
	immediateContext->CSSetShaderResources(3, 1, &srvSpotlightMaps);
	immediateContext->CSSetShaderResources(4, 1, &srvDirectionalMaps);
	immediateContext->CSSetShaderResources(5, 1, &srvSpotlight);
	immediateContext->CSSetShaderResources(6, 1, &srvDirectional);

	ID3D11Buffer* nrBuffer = scene->GetNrOfLightsBuffer();
	immediateContext->CSSetConstantBuffers(1, 1, &nrBuffer);

	immediateContext->Dispatch(WIDTH / 8, HEIGHT / 8, 1);

	immediateContext->CSSetShaderResources(0, 7, SRVnull);

	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &UAVnull, nullptr);

}