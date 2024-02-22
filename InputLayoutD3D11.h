#pragma once

#include <string>
#include <vector>

#include <d3d11_4.h>

class InputLayoutD3D11
{
private:
	std::vector<std::string> semanticNames; // Needed to store the semantic names of the element descs
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements;
	ID3D11InputLayout* inputLayout = nullptr;

public:
	InputLayoutD3D11() = default;
	~InputLayoutD3D11() ;
	InputLayoutD3D11(const InputLayoutD3D11& other) = delete;
	InputLayoutD3D11& operator=(const InputLayoutD3D11& other) = delete;
	InputLayoutD3D11(InputLayoutD3D11&& other) = delete;
	InputLayoutD3D11& operator=(InputLayoutD3D11&& other) = delete;

	void AddInputElement(const char* semanticName, DXGI_FORMAT format);
	void FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize);

	ID3D11InputLayout* GetInputLayout() const;
};