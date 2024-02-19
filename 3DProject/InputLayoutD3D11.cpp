#include "InputLayoutD3D11.h"


InputLayoutD3D11::~InputLayoutD3D11()
{
	//this->inputLayout->Release();
}

void InputLayoutD3D11::AddInputElement(const std::string& semanticName, DXGI_FORMAT format)
{
	static size_t cap = 0;
	this->semanticNames.push_back(semanticName);
	D3D11_INPUT_ELEMENT_DESC newElement = {semanticName.c_str(), 0, format, 0, cap, D3D11_INPUT_PER_VERTEX_DATA, 0};
	elements.push_back(newElement);
	cap += 12;
}

void InputLayoutD3D11::FinalizeInputLayout(ID3D11Device* device, const void* vsDataPtr, size_t vsDataSize)
{
	device->CreateInputLayout(this->elements.data(), this->elements.size(), vsDataPtr, vsDataSize, &this->inputLayout);
}

ID3D11InputLayout* InputLayoutD3D11::GetInputLayout() const
{
	return this->inputLayout;
}
