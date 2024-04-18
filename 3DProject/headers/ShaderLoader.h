#pragma once

#include "CameraD3D11.h"
#include "InputLayoutD3D11.h"
#include "ShaderD3D11.h"
#include "SamplerD3D11.h"
#include "VertexBufferD3D11.h"

bool ShaderLoader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& computeShader, ShaderD3D11 pixelShaders[2], ShaderD3D11 particleShaders[4],
	InputLayoutD3D11& inputLayout, SamplerD3D11& sampler, SamplerD3D11& shadowSampler, ShaderD3D11& hs, ShaderD3D11& ds);