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
    float4 tangent : SV_TARGET2;
    float4 ambientColour : SV_TARGET3;
    float4 diffuseColour : SV_TARGET4;
    float4 specular : SV_TARGET5;
    float4 normalMap : SV_TARGET6;
};

PSOutput main(PixelShaderInput input)
{
    PSOutput output;
    output.position = float4(input.worldPosition.xyz, input.uv.x);
    
    output.normal = normalize(float4(input.normal, 0));
    
    output.tangent = normalize(float4(input.tangent, 0));
        
    output.ambientColour = float4(ambientMap.Sample(textureSampler, input.uv).xyz, 1); //float4((output.normal + float4(1, 1, 1, 0)) / 2);
    output.diffuseColour = diffuseMap.Sample(textureSampler, input.uv);
    output.specular = float4(specularMap.Sample(textureSampler, input.uv).xyz, 1000); //Store exponent in alpha channel
    output.normalMap = normalMap.Sample(textureSampler, input.uv);

    return output;
}