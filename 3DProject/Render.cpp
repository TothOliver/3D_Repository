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
	const UINT WIDTH, const UINT HEIGHT, ID3D11DepthStencilView* dsView,  ID3D11InputLayout* inputLayout, ID3D11SamplerState* sampler, 
	D3D11_VIEWPORT viewport, ID3D11Buffer* vpBuffer, ID3D11Buffer* cameraPosition, std::vector<Scene*>& scenes, int sceneIndex, int uavSlot, ShaderD3D11 ps[2])
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

	immediateContext->VSSetConstantBuffers(1, 1, &vpBuffer);

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->PSSetSamplers(0, 1, &sampler);

	immediateContext->PSSetConstantBuffers(0, 1, &cameraPosition);

	immediateContext->RSSetViewports(1, &viewport);

	immediateContext->OMSetRenderTargets(3, rtv, dsView);
	scenes.at(sceneIndex)->DrawScene(immediateContext, ps, false);
	immediateContext->OMSetRenderTargets(3, RTVnull, nullptr);


	immediateContext->CSSetConstantBuffers(0, 1, &cameraPosition);

	//Window unordered access view
	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &uav, nullptr);

	//GBuffers shader resource view
	immediateContext->CSSetShaderResources(0, 3, srv);

	//For shadow mapping
	ID3D11ShaderResourceView* srvSpotlightMaps = scenes.at(sceneIndex)->GetShadowMapsSRV(SPOTLIGHT);
	ID3D11ShaderResourceView* srvDirectionalMaps = scenes.at(sceneIndex)->GetShadowMapsSRV(DIRECTIONAL);
	ID3D11ShaderResourceView* srvSpotlight = scenes.at(sceneIndex)->GetLightBufferSRV(SPOTLIGHT);
	ID3D11ShaderResourceView* srvDirectional = scenes.at(sceneIndex)->GetLightBufferSRV(DIRECTIONAL);
	immediateContext->CSSetShaderResources(3, 1, &srvSpotlightMaps);
	immediateContext->CSSetShaderResources(4, 1, &srvDirectionalMaps);
	immediateContext->CSSetShaderResources(5, 1, &srvSpotlight);
	immediateContext->CSSetShaderResources(6, 1, &srvDirectional);

	ID3D11Buffer* nrBuffer = scenes.at(sceneIndex)->GetNrOfLightsBuffer();
	immediateContext->CSSetConstantBuffers(1, 1, &nrBuffer);

	immediateContext->Dispatch(WIDTH / 8, HEIGHT / 8, 1);

	immediateContext->CSSetShaderResources(0, 7, SRVnull);

	ID3D11UnorderedAccessView* noUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &noUAV, nullptr);

}