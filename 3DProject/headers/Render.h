#pragma once
#include <d3d11.h>
#include "SceneManager.h"

void RenderToTargetUAV(ID3D11Device* device, ID3D11DeviceContext* immediateContext, ID3D11UnorderedAccessView* uav,
	const UINT WIDTH, const UINT HEIGHT, ID3D11DepthStencilView* dsView, ID3D11InputLayout* inputLayout,
	ID3D11SamplerState* sampler, D3D11_VIEWPORT viewport, ID3D11Buffer* vpBuffer, ID3D11Buffer* cameraPosition, std::vector<Scene*>& scenes, int sceneIndex, int uavSlot, ShaderD3D11 ps[2]);

void CreateGbuffers(ID3D11Device* device, ID3D11RenderTargetView** rtv, ID3D11ShaderResourceView** srv, UINT WIDTH, UINT HEIGHT);