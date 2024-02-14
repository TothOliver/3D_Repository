#include "ShaderD3D11.h"

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{
	Initialize(device, shaderType, dataPtr, dataSize);
}

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	Initialize(device, shaderType, csoPath);
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{
	std::vector<std::string> shaderFiles(6);
	shaderFiles = { "VertexShader.cso", "HullShader.cso", "DomainShader.cso", "GeometryShader.cso", "PixelShader.cso", "ComputeShader.cso" };
	char* bytes = { 0 };
	size_t fSize;
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	this->type = shaderType;
	std::string shaderData;
	std::ifstream reader;
	reader.open(csoPath, std::ios::binary | std::ios::ate);
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);

	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	switch (this->type)
	{
	case ShaderType::VERTEX_SHADER:
		device->CreateVertexShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.vertex);
		break;
	case ShaderType::HULL_SHADER:
		device->CreateHullShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.hull);
		break;
	case ShaderType::DOMAIN_SHADER:
		device->CreateDomainShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.domain);
		break;
	case ShaderType::GEOMETRY_SHADER:
		device->CreateGeometryShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.geometry); //device->CreateGeometryShaderWithStreamOutput()??
		break;
	case ShaderType::PIXEL_SHADER:
		device->CreatePixelShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.pixel);
		break;
	case ShaderType::COMPUTE_SHADER:
		device->CreateComputeShader(shaderData.c_str(), shaderData.length(), nullptr, &shader.compute);
		break;
	default:
		break;
	}

	shaderData.clear();
	reader.close();
}

void ShaderD3D11::BindShader(ID3D11DeviceContext* context) const
{

	switch (this->type)
	{
	case ShaderType::VERTEX_SHADER:
		context->VSSetShader(this->shader.vertex, nullptr, 0);
		break;
	case ShaderType::HULL_SHADER:
		context->HSSetShader(this->shader.hull, nullptr, 0);
		break;
	case ShaderType::DOMAIN_SHADER:
		context->DSSetShader(this->shader.domain, nullptr, 0);
		break;
	case ShaderType::GEOMETRY_SHADER:
		context->GSSetShader(this->shader.geometry, nullptr, 0);
		break;
	case ShaderType::PIXEL_SHADER:
		context->PSSetShader(this->shader.pixel, nullptr, 0);
		break;
	case ShaderType::COMPUTE_SHADER:
		context->CSSetShader(this->shader.compute, nullptr, 0);
		break;
	default:
		break;
	}
}
