RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D ambientMap : register(t2);
//Texture2D diffuseMap : register(t3);
//Texture2D specularMap : register(t4);
//Texture2D emissionMap : register(t5);
//Texture2D normalMap : register(t6);
SamplerState textureSampler;

[numthreads(8, 8, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	float3 position = positionGBuffer[DTid.xy].xyz;
	backBufferUAV[DTid.xy] = float4(ambientMap.SampleLevel(textureSampler, float2(positionGBuffer[DTid.xy].w, normalGBuffer[DTid.xy].w), 0));
}