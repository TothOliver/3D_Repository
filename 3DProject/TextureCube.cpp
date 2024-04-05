#pragma once

#include <d3d11.h>
#include "headers/TextureCube.h"
#include "headers/stb_image.h"

ID3D11Texture2D* CreateTexture(ID3D11Device* device, UINT sideLength, int arraysize, DXGI_FORMAT format, int bindFlags, int miscFlags)
{
	ID3D11Texture2D* texture;


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = sideLength;
	desc.Height = sideLength;
	desc.MipLevels = 1;
	desc.ArraySize = arraysize;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = miscFlags;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(hr))
	{
		throw std::runtime_error("Could not create texture2d");
	}

	return texture;
}

TextureCube::~TextureCube()
{
	//Check if cube was initialized

	if (cubeSRV != nullptr)
	{
		this->cubeSRV->Release();
	}

	if (cubeDSV != nullptr)
	{
		this->cubeDSV->Release();
	}

	for (size_t i = 0; i < 6; i++)
	{
		if (this->UAVCube[i] != nullptr)
		{
			this->UAVCube[i]->Release();
		}
	}
}

void TextureCube::Initialize(ID3D11Device* device, UINT sideLength, UINT nearZ, DirectX::XMFLOAT3 position)
{
	if (this->cubeType != NONE)
		throw "Cube type already set";
	this->cubeType = DYNAMIC;

	this->sideLength = sideLength;

	ID3D11Texture2D* DSVtexture = CreateTexture(device, sideLength, 1, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, 0);
	ID3D11Texture2D* cubeTexture = CreateTexture(device, sideLength, 6, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS, D3D11_RESOURCE_MISC_TEXTURECUBE);


	ProjectionInfo pi;
	pi.aspectRatio = 1;
	pi.fovAngleY = DirectX::XM_PIDIV2;
	pi.farZ = 1000;
	pi.nearZ = 0.01f;

	//Rotation values for each side of cube
	float upAngles[6] = { DirectX::XM_PIDIV2, -DirectX::XM_PIDIV2, 0.0f, 0.0f, 0.0f, DirectX::XM_PI }; // Rotate around up
	float rightAngles[6] = { 0.0f, 0.0f, -DirectX::XM_PIDIV2, DirectX::XM_PIDIV2, 0.0f, 0.0f }; // Rotate around right

	//UAV description for bind each slice of texture cube to 6 different unordered access views
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVdesc;
	UAVdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	UAVdesc.Texture2DArray.MipSlice = 0;
	UAVdesc.Texture2DArray.ArraySize = 1;

	ID3D11DeviceContext* immediateContext;
	device->GetImmediateContext(&immediateContext);

	for (int i = 0; i < 6; ++i)
	{
		UAVdesc.Texture2DArray.FirstArraySlice = i;
		HRESULT hr = device->CreateUnorderedAccessView(cubeTexture, &UAVdesc, &UAVCube[i]);

		if (FAILED(hr))
		{
			throw std::runtime_error("Could not create texture cube uav");
		}

		this->cameraCube[i].Initialize(device, pi, position);
		this->cameraCube[i].RotateUp(upAngles[i]);
		this->cameraCube[i].RotateRight(rightAngles[i]);
		this->cameraCube[i].UpdateInternalConstantBuffer(immediateContext);
	}

	HRESULT hr = device->CreateShaderResourceView(cubeTexture, nullptr, &this->cubeSRV);

	if (FAILED(hr))
		throw std::runtime_error("Could not create cube map srv");

	hr = device->CreateDepthStencilView(DSVtexture, nullptr, &this->cubeDSV);

	if (FAILED(hr))
		throw std::runtime_error("Could not creaate cube map dsv");

	//Set viewport dimensions and corner coordinates
	this->viewport.TopLeftX = 0;
	this->viewport.TopLeftY = 0;
	this->viewport.Width = sideLength;
	this->viewport.Height = sideLength;
	this->viewport.MinDepth = 0;
	this->viewport.MaxDepth = 1;

	cubeTexture->Release();
	DSVtexture->Release();
	immediateContext->Release();
}

ID3D11UnorderedAccessView* TextureCube::GetCubeUAV(int sideIndex) const
{
	return this->UAVCube[sideIndex];
}

ID3D11DepthStencilView* TextureCube::GetCubeDSV() const
{
	return this->cubeDSV;
}

ID3D11Buffer* TextureCube::GetCameraVPBuffer(int sideIndex) const
{
	return this->cameraCube[sideIndex].GetVPBuffer();
}

ID3D11Buffer* TextureCube::GetCameraPOSBuffer(int sideIndex) const
{
	return this->cameraCube[sideIndex].GetPositionBuffer();
}

UINT TextureCube::GetCubeSideLength() const
{
	return this->sideLength;
}

D3D11_VIEWPORT TextureCube::GetCubeViewport() const
{
	return this->viewport;
}

//void TextureCube::InitializeStaticCube(ID3D11Device* device, std::string filename)
//{
//	if (this->cubeType != NONE)
//		throw "Cube type already set";
//	this->cubeType = STATIC;
//
//	unsigned char* textures[6];
//	int width;
//	int height;
//
//	for (size_t i = 0; i < 6; i++)
//	{
//		textures[i] = LoadCubeData(filename, (TEXTURE_CUBE_FACE_INDEX)i, width, height);
//	}
//
//	ID3D11Texture2D* cube;
//
//	D3D11_SUBRESOURCE_DATA cubeData;
//	cubeData.pSysMem = textures[0];
//	cubeData.SysMemPitch = width * 4;
//	cubeData.SysMemSlicePitch = width * height * 4;
//
//	D3D11_TEXTURE2D_DESC desc;
//	ZeroMemory(&desc, sizeof(desc));
//	desc.Width = width;
//	desc.Height = height;
//	desc.MipLevels = 1;
//	desc.ArraySize = 6;
//	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	desc.SampleDesc.Count = 1;
//	desc.SampleDesc.Quality = 0;
//	desc.Usage = D3D11_USAGE_IMMUTABLE;
//	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//	desc.CPUAccessFlags = 0;
//	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
//
//	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
//	srvDesc.Format = desc.Format;
//	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
//	srvDesc.TextureCube.MipLevels = desc.MipLevels;
//	srvDesc.TextureCube.MostDetailedMip = 0;
//	
//	HRESULT hr = device->CreateTexture2D(&desc, &cubeData, &cube);
//	if (FAILED(hr))
//	{
//		throw std::runtime_error("Could not create texture cube");
//	}
//	if (cube != 0)
//	{
//		device->CreateShaderResourceView(cube, &srvDesc, &this->cubeSRV);
//	}
//
//	cube->Release();
//}

//unsigned char* TextureCube::LoadCubeData(std::string filename, TEXTURE_CUBE_FACE_INDEX face, int &width, int &height)
//{
//	int channels;
//	int desiredChannels = 4;
//	ID3D11Texture2D* texture;
//
//	std::string path = "assets/";
//	path += filename;
//
//	switch (face)
//	{
//	case POSITIVE_X:
//		path += " X.png";
//		break;
//	case NEGATIVE_X:
//		path += " -X.png";
//		break;
//	case POSITIVE_Y:
//		path += " Y.png";
//		break;
//	case NEGATIVE_Y:
//		path += " -Y.png";
//		break;
//	case POSITIVE_Z:
//		path += " Z.png";
//		break;
//	case NEGATIVE_Z:
//		path += " -Z.png";
//		break;
//	}
//
//	return stbi_load(path.c_str(), &width, &height, &channels, desiredChannels);
//}

ID3D11ShaderResourceView* TextureCube::GetCubeSRV() const
{
	return this->cubeSRV;
}