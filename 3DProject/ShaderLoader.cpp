#include "headers/ShaderLoader.h"


bool CreateShaders(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& computeShader, ShaderD3D11 pixelShaders[2], 
    ShaderD3D11 particleShaders[4], ShaderD3D11& hs, ShaderD3D11& ds)
{
    vertexShader.Initialize(device, ShaderType::VERTEX_SHADER, "shaders/VSDefault.cso");
    pixelShaders[0].Initialize(device, ShaderType::PIXEL_SHADER, "shaders/PSDefault.cso");
    pixelShaders[1].Initialize(device, ShaderType::PIXEL_SHADER, "shaders/PSReflections.cso");
    computeShader.Initialize(device, ShaderType::COMPUTE_SHADER, "shaders/CSDefault.cso");

    particleShaders[0].Initialize(device, ShaderType::COMPUTE_SHADER, "shaders/CSUpdateParticles.cso");
    particleShaders[1].Initialize(device, ShaderType::VERTEX_SHADER, "shaders/VSParticles.cso");
    particleShaders[2].Initialize(device, ShaderType::GEOMETRY_SHADER, "shaders/GSParticles.cso");
    particleShaders[3].Initialize(device, ShaderType::PIXEL_SHADER, "shaders/PSParticles.cso");

    hs.Initialize(device, ShaderType::HULL_SHADER, "shaders/HSDefault.cso");
    ds.Initialize(device, ShaderType::DOMAIN_SHADER, "shaders/DSDefault.cso");

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

bool CreateSampler(ID3D11Device* device, SamplerD3D11& sampler, SamplerD3D11& shadowSampler)
{
    std::optional<std::array<float, 4>> borderColour;
    borderColour = { 1, 1, 1, 1 };
    sampler.Initialize(device, D3D11_TEXTURE_ADDRESS_WRAP, borderColour, D3D11_FILTER_ANISOTROPIC);
    shadowSampler.Initialize(device, D3D11_TEXTURE_ADDRESS_CLAMP, borderColour, D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_COMPARISON_LESS_EQUAL);

    return true;
}

bool ShaderLoader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& computeShader, ShaderD3D11 pixelShaders[2], ShaderD3D11 particleShaders[4],
    InputLayoutD3D11& inputLayout, SamplerD3D11& textureSampler, SamplerD3D11& shadowSampler, ShaderD3D11& hs, ShaderD3D11& ds)
{

    CreateShaders(device, context, vertexShader, computeShader, pixelShaders, particleShaders, hs, ds);
    CreateInputLayout(inputLayout, device, vertexShader);
    CreateSampler(device, textureSampler, shadowSampler);

    return true;
}