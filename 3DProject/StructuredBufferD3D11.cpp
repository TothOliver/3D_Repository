#include "headers/StructuredBufferD3D11.h"

StructuredBufferD3D11::StructuredBufferD3D11(ID3D11Device* device, UINT sizeOfElement, size_t nrOfElementsInBuffer, void* bufferData, bool dynamic)
{
	Initialize(device, sizeOfElement, nrOfElementsInBuffer, bufferData, dynamic);
}

StructuredBufferD3D11::~StructuredBufferD3D11()
{
	if (this->buffer != nullptr)
	{
		this->buffer->Release();
		this->srv->Release();

		if (this->uav != nullptr)
		{
			this->uav->Release();
		}
	}
}

void StructuredBufferD3D11::Initialize(ID3D11Device* device, UINT sizeOfElement, size_t nrOfElementsInBuffer, void* bufferData, bool dynamic, bool hasUAV)
{
	this->elementSize = sizeOfElement;
	this->nrOfElements = nrOfElementsInBuffer;

	D3D11_BUFFER_DESC structuredBufferDesc;
	structuredBufferDesc.ByteWidth = this->elementSize * this->nrOfElements;
	structuredBufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
	
	if (hasUAV)
		structuredBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	structuredBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	structuredBufferDesc.BindFlags |= hasUAV ? D3D11_BIND_UNORDERED_ACCESS : 0;
	structuredBufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	structuredBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	structuredBufferDesc.StructureByteStride = this->elementSize;

	HRESULT hr = S_OK;

	if (bufferData != nullptr) {
		D3D11_SUBRESOURCE_DATA structuredData;
		structuredData.pSysMem = bufferData;
		structuredData.SysMemPitch = 0;
		structuredData.SysMemSlicePitch = 0;

		hr = device->CreateBuffer(&structuredBufferDesc, &structuredData, &this->buffer);
	}
	else {
		hr = device->CreateBuffer(&structuredBufferDesc, nullptr, &this->buffer);
	}

	if (FAILED(hr))
		throw ("Failed to create Structured Buffer");


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = this->nrOfElements;

	if (FAILED(device->CreateShaderResourceView(this->buffer, &srvDesc, &srv)))
	{
		throw ("Failed to create Structured Buffer SRV");
	}

	if (hasUAV)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = DXGI_FORMAT_UNKNOWN;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavDesc.Buffer.FirstElement = 0;
		uavDesc.Buffer.NumElements = nrOfElements;
		uavDesc.Buffer.Flags = 0;

		hr = device->CreateUnorderedAccessView(buffer, &uavDesc, &this->uav);

		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create uav for structured buffer");
		}

	}

}

void StructuredBufferD3D11::UpdateBuffer(ID3D11DeviceContext* context, void* data)
{
	//??
	context->Map(this->buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &this->mappedResource);
	memcpy(this->mappedResource.pData, data, this->elementSize);
	context->Unmap(this->buffer, 0);
}

UINT StructuredBufferD3D11::GetElementSize() const
{
	return this->elementSize;
}

size_t StructuredBufferD3D11::GetNrOfElements() const
{
	return this->nrOfElements;
}

ID3D11ShaderResourceView* StructuredBufferD3D11::GetSRV() const
{
	return this->srv;
}

ID3D11UnorderedAccessView* StructuredBufferD3D11::GetUAV() const
{
	return this->uav;
}

ID3D11Buffer* StructuredBufferD3D11::GetBuffer() const
{
	return this->buffer;
}
