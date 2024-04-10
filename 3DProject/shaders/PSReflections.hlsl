Texture2D ambientMap : register(t0);
Texture2D diffuseMap : register(t1);
Texture2D specularMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D heightMap : register(t4);

TextureCube cubeMap : register(t5);

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
    float4 diffuse : SV_TARGET2;
};

cbuffer CameraBuffer : register(b0)
{
    float4 camPosition;
};

PSOutput main(PixelShaderInput input)
{
    PSOutput output;
    
    //Calculate bitangent and transform the texture normal into world space (tangent is already in world space)
    float3 bitangent = normalize(cross(input.normal, input.tangent));
    float3x3 tbnMatrix = float3x3(normalize(input.tangent), bitangent, normalize(input.normal));
    
    //Parallax mapping
    float height = heightMap.Sample(textureSampler, input.uv).x;
    
    float3 V = normalize(mul(camPosition.xyz, transpose(tbnMatrix)) - mul(input.worldPosition.xyz, transpose(tbnMatrix)));
    float2 parallaxUV = input.uv + (height * 0.05 * V.xy) / V.z;
    
    float3 n = normalMap.Sample(textureSampler, parallaxUV).xyz;
    
    //Translate normal from colour space
    float3 mapNormal = float3(n.x * 2 - 1, n.y * 2 - 1, n.z * 2 - 1);
    
    output.normal = normalize(float4(mul(mapNormal, tbnMatrix), 0));
    
    //Calculate direction of reflected view
    float3 R = reflect(normalize(input.worldPosition.xyz - camPosition.xyz), output.normal.xyz);
    float4 reflection = cubeMap.Sample(textureSampler, R);    
    
    //Ambient constant is stored in position.w
    output.position = float4(input.worldPosition.xyz, ambientMap.Sample(textureSampler, parallaxUV).x);
    
    //Specular constant is stored in diffuse.w and reflection is added to colour
    output.diffuse = float4(diffuseMap.Sample(textureSampler, parallaxUV).xyz + reflection.xyz, specularMap.Sample(textureSampler, parallaxUV).x);

    return output;
}