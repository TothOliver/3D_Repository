cbuffer CameraPosition : register(b0)
{
    float4 camPos;
};

cbuffer MeshCenterPos : register(b1)
{
    float4 centerPos;
};

struct VertexShaderOutput
{
    float4 world_position : WORLD_POSITION;
    float4 position : SV_POSITION;
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

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(InputPatch<VertexShaderOutput, NUM_CONTROL_POINTS> ip)
{

    HS_CONSTANT_DATA_OUTPUT output;

    output.EdgeTessFactor[0] = output.EdgeTessFactor[1] = output.EdgeTessFactor[2] = output.InsideTessFactor = max(1, 100 - 4 * length(camPos - centerPos));

    return output;
}

struct HullShaderOutput
{
    float3 worldPos : WORLD_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]

HullShaderOutput main(InputPatch<VertexShaderOutput, NUM_CONTROL_POINTS> ip, uint i : SV_OutputControlPointID)
{

    HullShaderOutput output;

    output.worldPos = ip[i].world_position.xyz;
    output.normal = ip[i].normal;
    output.uv = ip[i].uv;
    output.tangent = ip[i].tangent;

    return output;
}


