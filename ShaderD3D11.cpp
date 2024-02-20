#include "ShaderD3D11.h"


ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize, const char* csoPath)
{
	Initialize(device, shaderType, dataPtr, dataSize);
}

ShaderD3D11::ShaderD3D11(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	Initialize(device, shaderType, csoPath);
}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const void* dataPtr, size_t dataSize)
{

	switch (shaderType)
	{
	case ShaderType::VERTEX_SHADER:
		device->CreateVertexShader(dataPtr, dataSize, nullptr, &shader.vertex);
		break;
	case ShaderType::HULL_SHADER:
		device->CreateHullShader(dataPtr, dataSize, nullptr, &shader.hull);
		break;
	case ShaderType::DOMAIN_SHADER:
		device->CreateDomainShader(dataPtr, dataSize, nullptr, &shader.domain);
		break;
	case ShaderType::GEOMETRY_SHADER:
		device->CreateGeometryShader(dataPtr, dataSize, nullptr, &shader.geometry); //device->CreateGeometryShaderWithStreamOutput()??
		break;
	case ShaderType::PIXEL_SHADER:
		device->CreatePixelShader(dataPtr, dataSize, nullptr, &shader.pixel);
		break;
	case ShaderType::COMPUTE_SHADER:
		device->CreateComputeShader(dataPtr, dataSize, nullptr, &shader.compute);
		break;
	default:
		break;
	}
	
	//CreateBlob(sizeof(shaderBlob), &shaderBlob);
	
	//memcpy(this->shaderBlob->GetBufferPointer(), dataPtr, dataSize);

}

void ShaderD3D11::Initialize(ID3D11Device* device, ShaderType shaderType, const char* csoPath)
{
	std::ifstream reader;
	reader.open(csoPath, std::ios::binary | std::ios::ate);
	if (!reader.is_open())
	{
		std::cerr << "Could not open VS file!" << std::endl;
	}
	reader.seekg(0, std::ios::end);
	shaderData.reserve(static_cast<unsigned int>(reader.tellg()));
	reader.seekg(0, std::ios::beg);
	shaderData.assign((std::istreambuf_iterator<char>(reader)), std::istreambuf_iterator<char>());

	Initialize(device, shaderType, shaderData.c_str(), shaderData.length());

	//shaderData.clear();
	//reader.close();
	
}

const void* ShaderD3D11::GetShaderByteData() const
{
	return this->shaderData.c_str();
}

size_t ShaderD3D11::GetShaderByteSize() const
{
	return this->shaderData.length();
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
