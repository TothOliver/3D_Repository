#include "ShaderLoader.h"


bool CreateShaders(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& pixelShader, std::string& vShaderByteCode)
{
    vertexShader.Initialize(device, ShaderType::VERTEX_SHADER, vShaderByteCode.c_str(), vShaderByteCode.length(), "VertexShader.cso");
    //pixelShader.Initialize(device, ShaderType::PIXEL_SHADER, vShaderByteCode.c_str(), vShaderByteCode.length(), "PixelShader.cso");

    vertexShader.BindShader(context);
    //pixelShader.BindShader(context);

    return true;
}

bool CreateInputLayout(InputLayoutD3D11& inputLayout, ID3D11Device* device, const std::string& vShaderByteCode)
{
    inputLayout.AddInputElement("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
    inputLayout.AddInputElement("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
    inputLayout.AddInputElement("UV", DXGI_FORMAT_R32G32_FLOAT);
    inputLayout.FinalizeInputLayout(device, vShaderByteCode.c_str(), vShaderByteCode.length());
    
    return true;
}

bool CreateTexture(ID3D11Device* device, ShaderResourceTextureD3D11& srt)
{
    const char* pathToTexture = "..\\..\\monkey_thumbs_up.jpg";
    srt.Initialize(device, pathToTexture);

    return true;
}

bool CreateSampler(ID3D11Device* device, SamplerD3D11& sampler)
{
    std::optional<std::array<float, 4>> borderColour;
    borderColour = { 1, 1, 1, 1 };
    sampler.Initialize(device, D3D11_TEXTURE_ADDRESS_CLAMP, borderColour);

    return true;
}

bool ShaderLoader(ID3D11Device* device, ID3D11DeviceContext* context, ShaderD3D11& vertexShader, ShaderD3D11& pixelShader,
    InputLayoutD3D11& inputLayout, ShaderResourceTextureD3D11& srt, SamplerD3D11& sampler)
{
    std::string vShaderByteCode;

    CreateShaders(device, context, vertexShader, pixelShader, vShaderByteCode);
    CreateInputLayout(inputLayout, device, vShaderByteCode);
    CreateTexture(device, srt);
    CreateSampler(device, sampler);

    return true;
}