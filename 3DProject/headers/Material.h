#pragma once

#include <d3d11.h>
#include <string>

struct Material
{

	ID3D11ShaderResourceView* ambientTexture = nullptr;
	ID3D11ShaderResourceView* diffuseTexture = nullptr;
	ID3D11ShaderResourceView* specularTexture = nullptr;
	ID3D11ShaderResourceView* normalTexture = nullptr;
	ID3D11ShaderResourceView* heightTexture = nullptr;

	std::string name;

	float specularExponent = 1;

};