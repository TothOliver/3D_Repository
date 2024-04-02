
struct VS_INPUT
{
    float4 pos : POSITIONT;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 tex : TEXCOORD;
    float4 posLight : LIGHTPOS;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    output.pos = nul(input.pos, world_matrix);
    output.normal = mul(input.normal, world_matrix);
    output.tex = input.tex;
    output.pos = mul(output.pos, vp_matrix);
    
    return output;
}