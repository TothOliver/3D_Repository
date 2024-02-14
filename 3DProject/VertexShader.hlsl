struct VertexShaderInput
{
	float3 position : POSITION;
	float3 colour : COLOUR;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
};

VertexShaderOutput main(VertexShaderInput input)
{
	VertexShaderOutput output;
	output.position = float4(input.position, 1.0f);
	return output;
}