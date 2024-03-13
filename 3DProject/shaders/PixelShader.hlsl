struct PixelShaderInput
{
    float4 worldPosition : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct PSOutput
{
    float4 position_u : SV_TARGET0;
    float4 normal_v : SV_TARGET1;
};

cbuffer LightBuffer : register(b0)
{
    float4x4 vpMatrix;
    float4 colour;
    float3 direction;
    float angle;
    float4 position;
    float4 intensity;
};

PSOutput main(PixelShaderInput input)
{
    PSOutput output;
    
    output.position_u = float4(input.worldPosition.xyz, input.uv.x);
    output.normal_v = float4(input.normal, input.uv.y);

    return output;
}