Texture2D displacementMap : register(t0);
sampler textureSampler;

cbuffer Camera : register(b0)
{
    float4x4 vp;
};

struct DomainShaderOutput
{
    float4 worldPos : WORLD_POSITION;
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct HullShaderOutput
{
    float3 worldPos : WORLD_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3
[domain("tri")]

DomainShaderOutput main(HS_CONSTANT_DATA_OUTPUT input, float3 barycentric : SV_DomainLocation, const OutputPatch<HullShaderOutput, NUM_CONTROL_POINTS> patch)
{

    DomainShaderOutput output;
    
    output.uv = patch[0].uv * barycentric.x + patch[1].uv * barycentric.y + patch[2].uv * barycentric.z;
    
    float displacement = 2 * displacementMap.SampleLevel(textureSampler, output.uv, 0).x;
    output.normal = normalize(patch[0].normal * barycentric.x + patch[1].normal * barycentric.y + patch[2].normal * barycentric.z);

    output.worldPos = float4(patch[0].worldPos * barycentric.x + patch[1].worldPos * barycentric.y + patch[2].worldPos * barycentric.z - displacement * output.normal, 1);
    output.tangent = normalize(patch[0].tangent * barycentric.x + patch[1].tangent * barycentric.y + patch[2].tangent * barycentric.z);
    
    output.position = mul(output.worldPos, vp);

    return output;
}