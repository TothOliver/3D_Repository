#pragma once

#include "MeshD3D11.h"
#include "Parser.h"
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void FillTexDesc(D3D11_TEXTURE2D_DESC &texDesc, int height, int width)
//Fills a texture2d description for creating a texture
{
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.ArraySize = 1;
	texDesc.Height = height;
	texDesc.Width = width;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.MiscFlags = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
}

void FillSubData(D3D11_SUBRESOURCE_DATA &subData, unsigned char* imageData, int width)
//Fills a subresource for creating a texture
{
	subData.pSysMem = imageData;
	subData.SysMemPitch = width * 4; //4 is the standard amount of colour channels used in this project
	subData.SysMemSlicePitch = 0;
}

ID3D11ShaderResourceView* CreateSRV(ID3D11Device* &device, unsigned char* imageData, int width, int height)
{
	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* texture;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SUBRESOURCE_DATA textureData;

	FillTexDesc(textureDesc, height, width);
	FillSubData(textureData, imageData, width);

	device->CreateTexture2D(&textureDesc, &textureData, &texture);
	device->CreateShaderResourceView(texture, nullptr, &srv);

	return srv;
}

int ParseObj(ID3D11Device* &device, std::string filename, std::vector<float>& vertexbuffer, std::vector<uint32_t>& indexbuffer, std::vector<SubMeshD3D11>& submeshes)
//Parses OBJ file and its MTL file if it exists
{
	
	std::vector<float> verticies;
	std::vector<float> normals;
	std::vector<float> uvCoordinates;
	std::vector<int> uvIndicies;
	std::vector<int> normalIndicies;

	std::vector<Material> materials;

	std::string currentMat = "default";
	size_t subMeshStartIndex = 0;
	size_t subMeshNrOfIndicies = 0;

	std::ifstream file(filename, std::ios::in);

	if (!file)
	{
		std::cerr << "Failed to open " << filename << std::endl;
		return -1;
	}

	std::string line;

	while (std::getline(file, line))
	{
		//Verticies
		if (line.substr(0, 2) == "v ")
		{
			//Find index of seperator between first and second value
			size_t index1 = line.find(' ', 2);
			verticies.push_back(std::stof(line.substr(2, index1 - 2)));

			//Find index of seperator between second and third value
			size_t index2 = line.find(' ', index1 + 1);
			verticies.push_back(std::stof(line.substr(index1, index2 - index1)));
			
			verticies.push_back(std::stof(line.substr(index2, line.length() - index2)));
		}

		//Texture Coordinates
		else if (line.substr(0, 2) == "vt")
		{
			//Find index of seperator between first and second value
			size_t index1 = line.find(' ', 3);
			uvCoordinates.push_back(std::stof(line.substr(3, index1 - 3)));

			uvCoordinates.push_back(std::stof(line.substr(index1, line.length() - index1)));
		}

		//Verticy Normals
		else if (line.substr(0, 2) == "vn")
		{
			//Find index of seperator between first and second value
			size_t index1 = line.find(' ', 3);

			normals.push_back(std::stof(line.substr(3, index1 - 3)));

			//Find index of seperator between second and third value
			size_t index2 = line.find(' ', index1 + 1);

			normals.push_back(std::stof(line.substr(index1, index2 - index1)));

			normals.push_back(std::stof(line.substr(index2, line.length() - index2)));
		}

		//Faces
		else if (line.substr(0, 2) == "f ")
		{
			std::string xyz[3];

			//Find index of seperator between first and second value substring
			size_t pos1 = line.find(' ', 2);
			xyz[0] = line.substr(2, pos1 - 2);

			//Find index of seperator between second and third value substring
			size_t pos2 = line.find(' ', pos1 + 1);
			xyz[1] = line.substr(pos1, pos2 - pos1);

			xyz[2] = line.substr(pos2, line.length() - pos2);

			for (size_t i = 0; i < 3; i++)
			{
				//Find index of seperator between first and second value
				pos1 = xyz[i].find('/');

				indexbuffer.push_back(std::stoi(xyz[i].substr(0, pos1)) - 1);

				//Find index of seperator between second and third value
				pos2 = xyz[i].find('/', pos1 + 1);

				uvIndicies.push_back(std::stoi(xyz[i].substr(pos1 + 1, pos2 - pos1 - 1)) - 1);

				normalIndicies.push_back(std::stoi(xyz[i].substr(pos2 + 1, xyz[i].length() - pos2)) - 1);

				//1 is subtracted since indicies in obj files start at index 1

				//Increase index amount for current submesh/material
				subMeshNrOfIndicies++;
			}
		}

		else if (line.substr(0, 6) == "usemtl")
		{
			if (currentMat != "default")
				for (size_t i = 0; i < materials.size(); i++)
					if (currentMat == materials.at(i).name)
					{
						submeshes.push_back(SubMeshD3D11(materials.at(i), subMeshStartIndex, subMeshNrOfIndicies));
						break;
					}

			currentMat = line.substr(7, line.length() - 7);
			subMeshStartIndex = indexbuffer.size() - 1;
			subMeshNrOfIndicies = 0;
		}

		//For filename of mtl file
		else if (line.substr(0, 6) == "mtllib")
		{
			ParseMtl(device, line.substr(7, line.length() - 7), materials);
		}

		//For usage of specific material
	}

	//For adding last submesh
	if (currentMat != "default")
		for (size_t i = 0; i < materials.size(); i++)
			if (currentMat == materials.at(i).name)
			{
				submeshes.push_back(SubMeshD3D11(materials.at(i), subMeshStartIndex, subMeshNrOfIndicies));
				break;
			}

	//Filling the vertexbuffer according to input layout
	for (size_t i = 0; i < indexbuffer.size(); i++)
	{
		vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3));
		vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3 + 1));
		vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3 + 2));

		if (!normals.empty())
		{
			vertexbuffer.push_back(normals.at(normalIndicies.at(i) * 3));
			vertexbuffer.push_back(normals.at(normalIndicies.at(i) * 3 + 1));
			vertexbuffer.push_back(normals.at(normalIndicies.at(i) * 3 + 2));
		}
		else
		{
			vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3));
			vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3 + 1));
			vertexbuffer.push_back(verticies.at(indexbuffer.at(i) * 3 + 2));
		}

		if (!uvCoordinates.empty())
		{
			vertexbuffer.push_back(uvCoordinates.at(uvIndicies.at(i) * 2));
			vertexbuffer.push_back(uvCoordinates.at(uvIndicies.at(i) * 2 + 1));
		}
		else
		{
			vertexbuffer.push_back(0);
			vertexbuffer.push_back(0);
		}
		
	}

	return 0;
}

int ParseMtl(ID3D11Device* &device, std::string filename, std::vector<Material>& materials)
{
	std::ifstream mtlFile(filename, std::ios::in);

	if (!mtlFile)
	{
		std::cerr << "Failed to open " << filename << std::endl;
		return -1;
	}

	std::string line;

	while (std::getline(mtlFile, line))
	{
		if (line.substr(0, 6) == "newmtl")
		{
			std::string matName = line.substr(7, line.length() - 7);

			Material newMat;
			newMat.name = matName;

			int width, height, channels;
			unsigned char* textureData;

			while (std::getline(mtlFile, line))
			{
				if (line.substr(0, 6) == "map_Ka")
				{
					textureData = stbi_load(line.substr(7, line.length() - 7).c_str(), &width, &height, &channels, 4);
					newMat.ambientTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 6) == "map_Kd")
				{
					textureData = stbi_load(line.substr(7, line.length() - 7).c_str(), &width, &height, &channels, 4);
					newMat.diffuseTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 6) == "map_Ks")
				{
					textureData = stbi_load(line.substr(7, line.length() - 7).c_str(), &width, &height, &channels, 4);
					newMat.specularTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 6) == "map_Ke")
				{
					textureData = stbi_load(line.substr(7, line.length() - 7).c_str(), &width, &height, &channels, 4);
					newMat.emissionTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 4) == "norm")
				{
					textureData = stbi_load(line.substr(5, line.length() - 5).c_str(), &width, &height, &channels, 4);
					newMat.normalTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 4) == "para")
				{
					textureData = stbi_load(line.substr(5, line.length() - 5).c_str(), &width, &height, &channels, 4);
					newMat.parallaxTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 4) == "disp")
				{
					textureData = stbi_load(line.substr(5, line.length() - 5).c_str(), &width, &height, &channels, 4);
					newMat.displacementTexture = CreateSRV(device, textureData, width, height);
				}
				else if (line.substr(0, 2) == "Ns")
				{
					newMat.specularExponent = std::stof(line.substr(3, line.length() - 3));
				}
				else if (line.empty())
					break;
			}

			materials.push_back(newMat);
		}
	}


	return 0;
}