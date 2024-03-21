RWTexture2D<unorm float4> backBufferUAV : register(u0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> ambientBuffer : register(t2);
Texture2D<float4> diffuseBuffer : register(t3);
Texture2D<float4> specularBuffer : register(t4);

struct SpotLight
{
    float4x4 vpMatrix;
    float4 colour;
    float3 direction;
    float angle;
    float4 position;
    float4 intensity;
};
StructuredBuffer<SpotLight> SpotLights : register(t5);

cbuffer cameraPos : register(b0)
{
    float4 cameraPosition;
};

cbuffer lightbuffer : register(b1)
{
    float4x4 vpMatrix;
    float4 colour;
    float3 direction;
    float angle;
    float4 position;
    float4 intensity;
};

cbuffer nrOfLightBuffer : register(b2)
{
    float nrOfLight;
};

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float ambient = 0;
    float diffuse = 0;
    float specular = 0;
    
    for (int p = 0; p < nrOfLight; p++)
    {

    }
    
    backBufferUAV[DTid.xy] = ambientBuffer[DTid.xy] * ambient + diffuseBuffer[DTid.xy] * diffuse + float4(1, 1, 1, 1) * specular;
}