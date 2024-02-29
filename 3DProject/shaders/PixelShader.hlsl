Texture2D ambientMap : register(T0);
Texture2D diffuseMap : register(T1);
Texture2D specularMap : register(T2);
Texture2D emissionMap : register(T3);
Texture2D normalMap : register(T4);
SamplerState samplersampler;

struct PixelShaderInput
{
    float4 world_position : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 sample = ambientMap.Sample(samplersampler, input.uv);
    return sample;
}