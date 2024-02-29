struct PixelShaderInput
{
    float4 world_position : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer campos : register(b0)
{
    float4x4 vp_matrix;
}

float4 main(PixelShaderInput input) : SV_TARGET
{
    float distance = length(input.world_position.xyz - transpose(vp_matrix)._41_42_43);
    if (distance < 8.1)
        return float4(1, 1, 1, 1);
    if (distance < 8.5)
        return float4(0.8, 0.8, 0.8, 1);
    if (distance < 9)
        return float4(0.6, 0.6, 0.6, 1);
    if (distance < 10)
        return float4(0.4, 0.4, 0.4, 1);
    if (distance < 12)
        return float4(0.2, 0.2, 0.2, 1);
    return float4(0, 0, 0, 1);
}