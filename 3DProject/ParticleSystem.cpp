#include "headers/ParticleSystem.h"
#include "headers/stb_image.h"
#include "headers/Parser.h"

#define PATH "assets/"
#define CONCAT(first, second) first second

Emitter::~Emitter()
{
	if (this->particleTexture != nullptr)
		this->particleTexture->Release();
}

void Emitter::Initialize(ID3D11Device* device, UINT nrOfParticles, DirectX::XMFLOAT3 position, float maxAngle, float maxDistance, float initialspeed, PARTICLE_TYPE type)
{
	this->nrOfParticles = nrOfParticles;

	int width, height, channels;
	int desiredChannels = 4;
	unsigned char* texData;

	if (type == SMOKE)
		texData = stbi_load(CONCAT(PATH, "smokeParticle.jpg"), &width, &height, &channels, desiredChannels);
	else
		texData = stbi_load(CONCAT(PATH, "rainParticle.jpg"), &width, &height, &channels, desiredChannels);

	this->particleTexture = CreateSRV(device, texData, width, height);

	stbi_image_free(texData);

	std::vector<ParticleData> data;
	for (size_t i = 0; i < nrOfParticles; i++)
	{
		ParticleData pData;

		pData.speed = initialspeed;

		if (type == SMOKE)
		{
			pData.maxDistance = float(rand() / float(RAND_MAX) * maxDistance);
			//Calculate random directional vector within cone with maxAngle
			float j = float(rand() / float(RAND_MAX) * DirectX::XM_2PI);
			float z = cos(maxAngle) + float(rand() / float(RAND_MAX) * (1 - cos(maxAngle)));

			float x = sqrt(1 - z * z) * cos(j);
			float y = sqrt(1 - z * z) * sin(j);

			pData.direction = DirectX::XMFLOAT3(x, y, z);

			pData.position = position;
			pData.startPosition = position;
		}
		else
		{
			pData.startPosition = DirectX::XMFLOAT3();

			pData.maxDistance = maxDistance;
			pData.direction = DirectX::XMFLOAT3(0, -1, 0);

			float x = float(rand() / float(RAND_MAX) * maxDistance) - maxDistance / 2;
			float y = float(rand() / float(RAND_MAX) * maxDistance);
			float z = float(rand() / float(RAND_MAX) * maxDistance) - maxDistance / 2;
			pData.position = DirectX::XMFLOAT3(x, y, z);
			pData.startPosition = DirectX::XMFLOAT3(x, maxDistance, z);
		}
		
		

		data.push_back(pData);
	}
	this->particleBuffer.Initialize(device, sizeof(ParticleData), nrOfParticles, data.data(), false, true);
}

ID3D11ShaderResourceView* Emitter::GetBufferSRV() const
{
	return this->particleBuffer.GetSRV();
}

ID3D11UnorderedAccessView* Emitter::GetBufferUAV() const
{
	return this->particleBuffer.GetUAV();
}

ID3D11ShaderResourceView* Emitter::GetTextureSRV() const
{
	return this->particleTexture;
}

UINT Emitter::GetNrOfParticles() const
{
	return this->nrOfParticles;
}