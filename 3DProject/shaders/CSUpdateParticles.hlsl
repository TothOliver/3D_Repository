struct Particle
{
    float3 position;
    float3 direction;
    float3 startPosition;
    float speed;
    float maxDistance;
};

RWStructuredBuffer<Particle> Particles : register(u0);

[numthreads(32, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    
    Particle currentParticle = Particles[DTid.x];
    
    if (length(currentParticle.position - currentParticle.startPosition) > currentParticle.maxDistance)
        currentParticle.position = currentParticle.startPosition;
    
    currentParticle.position += currentParticle.speed * currentParticle.direction;

    Particles[DTid.x] = currentParticle;
}