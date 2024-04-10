RWTexture2D<unorm float4> backBufferUAV : register(u0);
RWTexture2DArray<unorm float4> cubemapUAV : register(u1);

SamplerState shadowSampler : register(s0);

Texture2D<float4> positionBufferKa : register(t0);
Texture2D<float4> normalBuffer : register(t1);
Texture2D<float4> diffuseBufferKs : register(t2);

Texture2DArray<float> SpotshadowMaps : register(t3);
Texture2DArray<float> DirshadowMaps : register(t4);

struct Light
{
    float4x4 vpMatrix;
    float4 colour;
    float3 direction;
    float angle;
    float4 position;
    float4 intensity;
};
StructuredBuffer<Light> SpotLights : register(t5);
StructuredBuffer<Light> DirectionalLights : register(t6);

cbuffer cameraPos : register(b0)
{
    float4 cameraPosition;
}

cbuffer nrOfLightBuffer : register(b1)
{
    float2 nrOfSpotLight;
    float2 nrOfDirectionalLight;
};


[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    float4 pixelPosition = float4(positionBufferKa[DTid.xy].xyz, 1);
    float3 pixelNormal = normalBuffer[DTid.xy].xyz;

    float ambientMax = 0;
    
    float4 diffuse = float4(0, 0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);
    float specularKoefficient = 100;

    for (int p = 0; p < nrOfSpotLight[0]; p++)
    {
        float3 v = cameraPosition.xyz - positionBufferKa[DTid.xy].xyz;
        float3 V = normalize(v);
        
        float3 l = SpotLights[p].position.xyz - positionBufferKa[DTid.xy].xyz;
        float3 L = normalize(l);
        float3 h = V + L;
        float3 H = normalize(h / dot(h, h));

        float teta = dot(L, normalBuffer[DTid.xy].xyz);
        float delta = dot(H, normalBuffer[DTid.xy].xyz);
        
        float3 D = normalize(SpotLights[p].direction);
        float cosAngle = dot(-L, D);
        float spotEffect = smoothstep(SpotLights[p].angle, 0, acos(cosAngle));

        //Calculate fragements if they are in shadow or not
        float4 shadowCoordinates = mul(float4(pixelPosition.xyz, 1), SpotLights[p].vpMatrix);
        shadowCoordinates.xyz /= shadowCoordinates.w;
        float3 shadowMapUV = float3(shadowCoordinates.x * 0.5f + 0.5, shadowCoordinates.y * -0.5f + 0.5f, p);
        float shadowDepth = SpotshadowMaps.SampleLevel(shadowSampler, shadowMapUV, 0).r + 0.001;

        float shadowFactor = (shadowCoordinates.z > shadowDepth) ? 0.0f : 1.0f;
        
        if (SpotLights[p].intensity[0] > ambientMax)
            ambientMax = SpotLights[p].intensity[0];

        diffuse += shadowFactor * float4(diffuseBufferKs[DTid.xy].xyz, 0) * SpotLights[p].intensity[1] * saturate(teta) * spotEffect / dot(l, l);
        specular += shadowFactor * float4(1, 1, 1, 0) * diffuseBufferKs[DTid.xy].a * SpotLights[p].intensity[2] * pow(saturate(delta), specularKoefficient) * spotEffect / dot(l, l);
    }
    
    for (int d = 0; d < nrOfDirectionalLight[0]; d++)
    {
        float3 D = normalize(DirectionalLights[d].direction.xyz);
        float3 V = normalize(cameraPosition.xyz - positionBufferKa[DTid.xy].xyz);
        
        float3 h = V + D;
        float3 H = normalize(h / dot(h, h));
        float delta = dot(H, normalBuffer[DTid.xy].xyz);

        float4 shadowCoordinates = mul(float4(pixelPosition.xyz, 1), DirectionalLights[d].vpMatrix);
        shadowCoordinates.xyz /= shadowCoordinates.w;
        float3 shadowMapUV = float3(shadowCoordinates.x * 0.5f + 0.5, shadowCoordinates.y * -0.5f + 0.5f, d);
        float shadowDepth = DirshadowMaps.SampleLevel(shadowSampler, shadowMapUV, 0).r + 0.01;

        float shadowFactor = (shadowCoordinates.z > shadowDepth) ? 0.0f : 1.0f;
        
        if (DirectionalLights[d].intensity[0] > ambientMax)
            ambientMax = DirectionalLights[d].intensity[0];
        
        //Diffuse and specular components are stored in the same gbuffer
        diffuse += shadowFactor * float4(diffuseBufferKs[DTid.xy].xyz, 0) * max(dot(D, normalBuffer[DTid.xy].xyz), 0.0) * DirectionalLights[d].intensity[1];
        specular += shadowFactor * float4(1, 1, 1, 0) * diffuseBufferKs[DTid.xy].a * DirectionalLights[d].intensity[2] * pow(saturate(delta), specularKoefficient);
    }
    
    float4 ambient = positionBufferKa[DTid.xy].a * ambientMax * float4(diffuseBufferKs[DTid.xy].xyz, 0);
    backBufferUAV[DTid.xy] = ambient + diffuse + specular;
    cubemapUAV[DTid.xyz] = ambient + diffuse + specular;
}

//[numthreads(8, 8, 1)]
//void main(uint3 DTid : SV_DispatchThreadID)
//{
    
//    //float specularExponent = specularBuffer[DTid.xy].a;
    
//    float Ks = diffuseBufferKs[DTid.xy].w;
//    float Ka = positionBufferKa[DTid.xy].w;
    
//    float3 normal = normalBuffer[DTid.xy].xyz;
    
    
    
//    float3 position = positionBufferKa[DTid.xy].xyz;
    
//    float l = length(position - cameraPosition.xyz);
    
//    float3 r = cameraPosition.xyz - position;
//    float3 L = normalize(r);
//    float4 diffuse = diffuseBufferKs[DTid.xy] * saturate(dot(L, normal)); // dot(r, r);
    
//    //Specular component
//    float3 R = normalize(reflect(-L, normal));
//    float3 V = normalize(cameraPosition.xyz - position);
//    float4 specular = float4(1, 1, 1, 1) * Ks * pow(saturate(dot(R, V)), 100); // dot(r, r);
//    float4 ambient = diffuseBufferKs[DTid.xy] * Ka;
    
//    backBufferUAV[DTid.xy] = Ka * diffuseBufferKs[DTid.xy] * 0.5f + specular;
//    cubemapUAV[DTid.xyz] = ambient + diffuse + specular;

//}