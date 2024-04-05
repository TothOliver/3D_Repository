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
	ID3D11RenderTargetView* rtv[7];
	ID3D11ShaderResourceView* srv[7];

	GBuffer positionBuffer(device, WIDTH, HEIGHT);
	GBuffer normalBuffer(device, WIDTH, HEIGHT);
	GBuffer tangentBuffer(device, WIDTH, HEIGHT);
	GBuffer ambientBuffer(device, WIDTH, HEIGHT);
	GBuffer diffuseBuffer(device, WIDTH, HEIGHT);
	GBuffer specularBuffer(device, WIDTH, HEIGHT);
	GBuffer normalMapBuffer(device, WIDTH, HEIGHT);

	rtv[0] = positionBuffer.GetRTV();
	rtv[1] = normalBuffer.GetRTV();
	rtv[2] = tangentBuffer.GetRTV();
	rtv[3] = ambientBuffer.GetRTV();
	rtv[4] = diffuseBuffer.GetRTV();
	rtv[5] = specularBuffer.GetRTV();
	rtv[6] = normalMapBuffer.GetRTV();

	srv[0] = positionBuffer.GetSRV();
	srv[1] = normalBuffer.GetSRV();
	srv[2] = tangentBuffer.GetSRV();
	srv[3] = ambientBuffer.GetSRV();
	srv[4] = diffuseBuffer.GetSRV();
	srv[5] = specularBuffer.GetSRV();
	srv[6] = normalMapBuffer.GetSRV();

	ID3D11RenderTargetView* RTVnull[7];
	ID3D11ShaderResourceView* SRVnull[7];

	float clear[4] = { 1, 0.2, 0.2, 1 };

	for (size_t i = 0; i < 7; i++)
	{
		RTVnull[i] = nullptr;
		SRVnull[i] = nullptr;
		immediateContext->ClearRenderTargetView(rtv[i], clear);
	}

	immediateContext->ClearUnorderedAccessViewFloat(uav, clear);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

	immediateContext->VSSetConstantBuffers(1, 1, &vpBuffer);

	immediateContext->IASetInputLayout(inputLayout);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	immediateContext->PSSetSamplers(0, 1, &sampler);

	immediateContext->PSSetConstantBuffers(0, 1, &cameraPosition);
	immediateContext->CSSetSamplers(0, 1, &sampler);

	immediateContext->RSSetViewports(1, &viewport);

	immediateContext->OMSetRenderTargets(7, rtv, dsView);
	scenes.at(sceneIndex)->DrawScene(immediateContext, ps);
	immediateContext->OMSetRenderTargets(7, RTVnull, nullptr);

	immediateContext->CSSetShaderResources(0, 7, srv);
	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &uav, nullptr);
	immediateContext->CSSetConstantBuffers(0, 1, &cameraPosition);
	immediateContext->Dispatch(WIDTH / 8, HEIGHT / 8, 1);
	immediateContext->CSSetShaderResources(0, 7, SRVnull);

	ID3D11UnorderedAccessView* noUAV = nullptr;
	immediateContext->CSSetUnorderedAccessViews(uavSlot, 1, &noUAV, nullptr);

}