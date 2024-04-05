#pragma once
#include <d3d11_4.h>
#include "CameraD3D11.h"
#include <string>

enum TEXTURE_CUBE_FACE_INDEX
{
	POSITIVE_X = 0,
	NEGATIVE_X = 1,
	POSITIVE_Y = 2,
	NEGATIVE_Y = 3,
	POSITIVE_Z = 4,
	NEGATIVE_Z = 5
};

enum TEXTURE_CUBE_TYPE
{
	NONE = 0,
	STATIC = 1,
	DYNAMIC = 2
};

class TextureCube
{
private:
	ID3D11UnorderedAccessView* UAVCube[6];
	ID3D11ShaderResourceView* cubeSRV;
	ID3D11DepthStencilView* cubeDSV;
	CameraD3D11 cameraCube[6];
	TEXTURE_CUBE_TYPE cubeType = NONE;
	UINT sideLength;
	D3D11_VIEWPORT viewport;

	unsigned char* LoadCubeData(std::string filename, TEXTURE_CUBE_FACE_INDEX face, int &width, int &height) = delete;

public:
	TextureCube() = default;
	~TextureCube();
	void Initialize(ID3D11Device* device, UINT sideLength, UINT nearZ, DirectX::XMFLOAT3 position);
	void InitializeStaticCube(ID3D11Device* device, std::string filename) = delete;

	ID3D11ShaderResourceView* GetCubeSRV() const;
	ID3D11UnorderedAccessView* GetCubeUAV(int sideIndex) const;
	ID3D11DepthStencilView* GetCubeDSV() const;
	ID3D11Buffer* GetCameraVPBuffer(int sideIndex) const;
	ID3D11Buffer* GetCameraPOSBuffer(int sideIndex) const;
	UINT GetCubeSideLength() const;
	D3D11_VIEWPORT GetCubeViewport() const;
};