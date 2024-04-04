#include "headers/ShaderLoader.h"


bool CreateShaders(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& computeShader, ShaderD3D11& pixelShader, InputLayoutD3D11& inputLayout)
{
    vertexShader.Initialize(device, ShaderType::VERTEX_SHADER, "shaders/VertexShader.cso");
    computeShader.Initialize(device, ShaderType::COMPUTE_SHADER, "shaders/ComputeShader.cso");
    pixelShader.Initialize(device, ShaderType::PIXEL_SHADER, "shaders/PixelShader.cso");

    return true;
}

bool CreateInputLayout(InputLayoutD3D11& inputLayout, ID3D11Device* device, ShaderD3D11& vertexShader)
{
    inputLayout.AddInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
    inputLayout.AddInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
    inputLayout.AddInputElement("TANGENT", DXGI_FORMAT_R32G32B32_FLOAT);
    inputLayout.AddInputElement("UV", DXGI_FORMAT_R32G32_FLOAT);
    inputLayout.FinalizeInputLayout(device, vertexShader.GetShaderByteData(), vertexShader.GetShaderByteSize());
    
    return true;
}

bool CreateSampler(ID3D11Device* device, SamplerD3D11& sampler, SamplerD3D11& samplerShadow)
{
    std::optional<std::array<float, 4>> borderColour;
    borderColour = { 1, 1, 1, 1 };

    sampler.Initialize(device, D3D11_TEXTURE_ADDRESS_CLAMP, borderColour, D3D11_FILTER_ANISOTROPIC);
    samplerShadow.Initialize(device, D3D11_TEXTURE_ADDRESS_CLAMP, borderColour, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_COMPARISON_LESS_EQUAL);//only this

    return true;
}

bool ShaderLoader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& computeShader, ShaderD3D11& pixelShader,
    InputLayoutD3D11& inputLayout, SamplerD3D11& sampler, SamplerD3D11& samplerShadow)
{

    CreateShaders(device, context, vertexShader, computeShader, pixelShader, inputLayout);
    CreateInputLayout(inputLayout, device, vertexShader);
    //CreateTexture(device, srt);
    CreateSampler(device, sampler, samplerShadow);

    return true;
}