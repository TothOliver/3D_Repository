struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 world_position : WORLD_POSITION;
	float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

cbuffer world : register(b0)
{
    float4x4 world_matrix;
}

cbuffer vp : register(b1)
{
    float4x4 view_projection;
}

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    output.world_position = mul(float4(input.position, 1.0f), world_matrix);
    output.position = mul(output.world_position, view_projection);
    output.normal = input.normal;
    output.uv = input.uv;
	return output;
}