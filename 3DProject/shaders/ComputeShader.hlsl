RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> tangentBuffer : register(t2);
Texture2D<float4> ambientBuffer : register(t3);
Texture2D<float4> diffuseBuffer : register(t4);
Texture2D<float4> specularBuffer : register(t5);
Texture2D<float4> normalMapBuffer : register(t6);

cbuffer cameraPos : register(b0)
{
    float4 cameraPosition;
}

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float specularExponent = specularBuffer[DTid.xy].w;
    
    //Calculate bitangent and transform the texture normal into world space (tangent is already in world space)
    float3 bitangent = normalize(cross(normalGBuffer[DTid.xy].xyz, tangentBuffer[DTid.xy].xyz));
    float3x3 tbnMatrix = float3x3(tangentBuffer[DTid.xy].xyz, bitangent, normalize(normalGBuffer[DTid.xy].xyz));
    float3 faceNormal = float3(normalMapBuffer[DTid.xy].x * 2 - 1, normalMapBuffer[DTid.xy].y * 2 - 1, normalMapBuffer[DTid.xy].z * 2 - 1);
    
    float3 normal = normalize(float4(mul(faceNormal, tbnMatrix), 0));
    
	float3 position = positionGBuffer[DTid.xy].xyz;
    float l = length(positionGBuffer[DTid.xy].xyz - cameraPosition.xyz);
    
    float3 r = cameraPosition.xyz - position;
    float3 L = normalize(r);
    float diffuse = diffuseBuffer[DTid.xy].xyz * saturate(dot(L, normal)); // / dot(r, r);
    
    //Specular component
    float3 R = normalize(reflect(-L, normal));
    float3 V = normalize(cameraPosition.xyz - position);
    float specular = specularBuffer[DTid.xy].xyz * pow(saturate(dot(R, V)), specularBuffer[DTid.xy].w); // / dot(r, r);
    
    backBufferUAV[DTid.xy] = ambientBuffer[DTid.xy] * 0.2f + specularBuffer[DTid.xy] * specular;
   // backBufferUAV[DTid.xy] = float4(normal.x + 1, normal.y + 1, normal.z + 1, 0) * 0.5;

}