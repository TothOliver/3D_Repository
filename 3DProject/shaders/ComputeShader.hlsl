RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> ambientBuffer : register(t2);
Texture2D<float4> diffuseBuffer : register(t3);
Texture2D<float4> specularBuffer : register(t4);

cbuffer cameraPos : register(b0)
{
    float4 cameraPosition;
}

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    
	float3 position = positionGBuffer[DTid.xy].xyz;
    float l = length(positionGBuffer[DTid.xy].xyz - cameraPosition.xyz);
    
    float3 r = cameraPosition.xyz - position;
    float3 L = normalize(r);
    float diffuse = diffuseBuffer[DTid.xy].xyz * saturate(dot(L, normalGBuffer[DTid.xy].xyz)); // / dot(r, r);
    
    //Specular component
    float3 R = normalize(reflect(-L, normalGBuffer[DTid.xy].xyz));
    float3 V = normalize(cameraPosition.xyz - position);
    float specular = specularBuffer[DTid.xy].xyz * pow(saturate(dot(R, V)), 100); // / dot(r, r);
    
    backBufferUAV[DTid.xy] = ambientBuffer[DTid.xy] * 0.2f + ambientBuffer[DTid.xy] * specular;

}