struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
    float2 uv : UV;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer wvp : register(b0)
{
    float4x4 world_matrix;
    float4x4 vp_matrix;
}

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    output.position = mul(mul(float4(input.position, 1.0f), transpose(world_matrix)), transpose(vp_matrix));
    output.normal = input.normal;
    output.uv = input.uv;
	return output;
}