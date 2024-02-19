#pragma once

#include "CameraD3D11.h"
#include "InputLayoutD3D11.h"
#include "ShaderD3D11.h"
#include "ShaderResourceTextureD3D11.h"
#include "SamplerD3D11.h"

bool ShaderLoader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& pixelShader,
	InputLayoutD3D11& inputLayout, ShaderResourceTextureD3D11& srt, SamplerD3D11& sampler);