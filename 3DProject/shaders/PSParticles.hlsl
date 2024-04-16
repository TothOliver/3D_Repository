Texture2D particleTexture : register(t0);

SamplerState textureSampler;

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 uv : UV;
};

struct PSOutput
{
    float4 position : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 colour : SV_TARGET2;
};

PSOutput main(PixelShaderInput input)
{
    PSOutput output;

    output.position = float4(input.position.xyz, 1);
    output.normal = float4(0, 0, 0, 0);
    output.colour = particleTexture.Sample(textureSampler, input.uv);

    return output;
}