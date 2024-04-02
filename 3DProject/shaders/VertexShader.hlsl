struct VertexShaderInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
};

struct VertexShaderOutput
{
    float4 world_position : WORLD_POSITION;
	float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : UV;
    float4 posLight : LIGHTPOS;
};

cbuffer world : register(b0)
{
    float4x4 world_matrix;
}

cbuffer vp : register(b1)
{
    float4x4 view_projection;
}

cbuffer light : register(b2)
{
    float4x4 light_vp;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
    output.world_position = mul(float4(input.position, 1.0f), world_matrix);
    output.position = mul(output.world_position, view_projection);
    output.normal = normalize(mul(float4(input.normal, 0), world_matrix));
    output.tangent = normalize(mul(float4(input.tangent, 0), world_matrix));
    
    output.uv = input.uv;
    output.posLight = mul(output.position, light_vp);
    
	return output;
}