#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <iostream>

#include "RenderTargetD3D11.h"

bool SetupD3D11(UINT width, UINT height, HWND window, ID3D11Device*& device, ID3D11DeviceContext*& immediateContext, IDXGISwapChain*& swapChain,
	ID3D11UnorderedAccessView*& uav, ID3D11DepthStencilView*& dsView, D3D11_VIEWPORT& viewport);