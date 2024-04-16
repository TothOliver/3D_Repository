struct Particle
{
    float3 position;
    float3 direction;
    float3 startPosition;
    float speed;
    float maxDistance;
};

StructuredBuffer<Particle> Particles : register(t0);

float3 main(uint vertexID : SV_VertexID) : POSITION
{
    return Particles[vertexID].position;
}