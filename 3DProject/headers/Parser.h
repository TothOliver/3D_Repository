#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include "SubMeshD3D11.h"
#include "Material.h"



int ParseObj(ID3D11Device*& device, std::string filename, std::vector<float>& vertexbuffer, std::vector<uint32_t>& indexbuffer, std::vector<SubMeshD3D11>& submeshes);

int ParseMtl(ID3D11Device* &device, std::string filename, std::vector<Material>& materials);

ID3D11ShaderResourceView* CreateSRV(ID3D11Device*& device, unsigned char* imageData, int width, int height);

