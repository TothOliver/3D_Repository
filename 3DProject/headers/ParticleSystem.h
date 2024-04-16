#pragma once
#include <vector>
#include <DirectXMath.h>
#include "StructuredBufferD3D11.h"

enum PARTICLE_TYPE
{
	SMOKE,
	RAIN
};

struct ParticleData
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 direction;
	DirectX::XMFLOAT3 startPosition;
	float speed;
	float maxDistance;
};

class Emitter
{
private:
	StructuredBufferD3D11 particleBuffer;
	DirectX::XMFLOAT3 position;
	float maxAngle;
	UINT nrOfParticles = 0;
	ID3D11ShaderResourceView* particleTexture;

public:
	Emitter() = default;
	~Emitter();
	void Initialize(ID3D11Device* device, UINT nrOfParticles, DirectX::XMFLOAT3 position, float maxAngle, float lifespan, float initialspeed, PARTICLE_TYPE type);

	ID3D11ShaderResourceView* GetBufferSRV() const;

	ID3D11UnorderedAccessView* GetBufferUAV() const;

	ID3D11ShaderResourceView* GetTextureSRV() const;

	UINT GetNrOfParticles() const;
};