RWTexture2D<unorm float4> backBufferUAV : register(u0);
SamplerComparisonState shadowSampler : register(s0);

Texture2D<float4> positionGBuffer : register(t0);
Texture2D<float4> normalGBuffer : register(t1);
Texture2D<float4> ambientBuffer : register(t2);
Texture2D<float4> diffuseBuffer : register(t3);
Texture2D<float4> specularBuffer : register(t4);
Texture2DArray<float> shadowMaps : register(t5); //not working, need information form pixel/vertex

struct SpotLight
{
    float4x4 vpMatrix;
    float4 colour;
    float3 direction;
    float angle;
    float4 position;
    float4 intensity;
};
StructuredBuffer<SpotLight> SpotLights : register(t6);
StructuredBuffer<SpotLight> DirectionalLights : register(t7);

cbuffer cameraPos : register(b0)
{
    float4 cameraPosition;
};

cbuffer nrOfLightBuffer : register(b1)
{
    float2 nrOfSpotLight;
    float2 nrOfDirectionalLight;
};

cbuffer light : register(b2)
{
    float4x4 light_vp;
};

[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float4 pixelPosition = positionGBuffer[DTid.xy];
    float3 pixelNormal = normalGBuffer[DTid.xy].xyz;

    float4 ambient = ambientBuffer[DTid.xy] * SpotLights[0].intensity[0];
    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);
    float specularKoefficient = 100;

    for (int p = 0; p < nrOfSpotLight[0]; p++)
    {
        float4x4 vpShadow = SpotLights[p].vpMatrix;

        float3 v = cameraPosition.xyz - positionGBuffer[DTid.xy].xyz;
        float3 V = normalize(v);
        
        float3 l = SpotLights[p].position.xyz - positionGBuffer[DTid.xy].xyz;
        float3 L = normalize(l);
        float3 h = V + L;
        float3 H = normalize(h / dot(h, h));

        float teta = dot(L, normalGBuffer[DTid.xy].xyz);
        float delta = dot(H, normalGBuffer[DTid.xy].xyz);
        
        float3 D = normalize(SpotLights[p].direction);
        float cosAngle = dot(-L, D);
        float spotEffect = smoothstep(SpotLights[p].angle, 0, acos(cosAngle));

        
        float4 shadowCoordinates = mul(SpotLights[p].vpMatrix, pixelPosition);
        shadowCoordinates.xyz /= shadowCoordinates.w;
        float shadowDepth = shadowMaps.SampleCmpLevelZero(shadowSampler, float3(shadowCoordinates.xy, 0), shadowCoordinates.z).r;

        float shadowFactor = (shadowCoordinates.z < shadowDepth) ? 1.0f : 0.0f;

        diffuse += shadowFactor * diffuseBuffer[DTid.xy] * SpotLights[p].intensity[1] * saturate(teta) * spotEffect/ dot(l, l);
        specular += shadowFactor * specularBuffer[DTid.xy] * SpotLights[p].intensity[2] * pow(saturate(delta), specularKoefficient) * spotEffect / dot(l, l);
    }
    
    for (int d = 0; d < nrOfDirectionalLight[0]; d++)
    {
        float3 D = normalize(DirectionalLights[d].direction.xyz);

        
        float3 V = normalize(cameraPosition.xyz - positionGBuffer[DTid.xy].xyz);
        
        float3 H = normalize(V - D);
        float delta = dot(H, normalGBuffer[DTid.xy].xyz);

        float4 shadowCoordinates = mul(SpotLights[p].vpMatrix, pixelPosition);
        shadowCoordinates.xyz /= shadowCoordinates.w;
        float shadowDepth = shadowMaps.SampleCmpLevelZero(shadowSampler, float3(shadowCoordinates.xy, 0), shadowCoordinates.z).r;

        float shadowFactor = (shadowCoordinates.z < shadowDepth) ? 1.0f : 0.0f;
        shadowFactor = 1;
        diffuse += shadowFactor * diffuseBuffer[DTid.xy] * max(dot(D, normalGBuffer[DTid.xy].xyz), 0.0) * DirectionalLights[d].intensity[1];
        specular += shadowFactor * specularBuffer[DTid.xy] * DirectionalLights[d].intensity[2] * pow(saturate(delta), specularKoefficient);
    }
    
    backBufferUAV[DTid.xy] = ambient + diffuse + specular;

}