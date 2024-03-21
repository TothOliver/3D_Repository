Texture2D ambientMap : register(t2);
Texture2D diffuseMap : register(t3);
Texture2D specularMap : register(t4);
Texture2D normalMap : register(t5);

SamplerState textureSampler;

struct PixelShaderInput
{
    float4 worldPosition : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct PSOutput
{
    float4 position : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 ambientColour : SV_TARGET2;
    float4 diffuseColour : SV_TARGET3;
    float4 specular : SV_TARGET4;
};

cbuffer lightbuffer : register(b0)
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
    output.position = float4(input.worldPosition.xyz, input.uv.x);
    
    //Calculate bitangent and transform the texture normal into world space (tangent is already in world space)
    float3 bitangent = normalize(cross(input.normal, input.tangent));
    float3x3 tbnMatrix = float3x3(input.tangent, bitangent, input.normal);
    float3 RGBnormal = normalMap.Sample(textureSampler, input.uv);
    float3 faceNormal = float3(RGBnormal.x * 2 - 1, RGBnormal.y * 2 - 1, RGBnormal.z * 2 - 1);
    
    output.normal = float4(mul(faceNormal, tbnMatrix), 1);
    
    output.ambientColour = ambientMap.Sample(textureSampler, input.uv) * intensity[0]; //float4((output.normal + float4(1, 1, 1, 0)) / 2);
    output.diffuseColour = diffuseMap.Sample(textureSampler, input.uv);
    output.specular = specularMap.Sample(textureSampler, input.uv);

    return output;
}