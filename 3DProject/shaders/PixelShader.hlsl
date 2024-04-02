Texture2D ambientMap : register(t2);
Texture2D diffuseMap : register(t3);
Texture2D specularMap : register(t4);
Texture2D normalMap : register(t5);
Texture2DArray<float> shadowMaps : register(t6);

SamplerState textureSampler : register(s0);
SamplerComparisonState shadowSampler : register(s1);

struct PixelShaderInput
{
    float4 worldPosition : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
    float4 posLight : LIGHTPOS;
};

struct PSOutput
{
    float4 position : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 ambientColour : SV_TARGET2;
    float4 diffuseColour : SV_TARGET3;
    float4 specular : SV_TARGET4;
    float4 shadow : SV_TARGET5;
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

    //Calculate fragements if they are in shadow or not
    float4 shadowCoordinates = input.posLight;
    shadowCoordinates.xyz /= shadowCoordinates.w;
    float shadowDepth = shadowMaps.SampleCmpLevelZero(shadowSampler, float3(shadowCoordinates.xy, 0), shadowCoordinates.z).r;

    float shadowFactor = (shadowCoordinates.z < shadowDepth) ? 1.0f : 0.0f;
 
    output.ambientColour = ambientMap.Sample(textureSampler, input.uv);
    output.diffuseColour = diffuseMap.Sample(textureSampler, input.uv);
    output.specular = specularMap.Sample(textureSampler, input.uv);

    return output;
}