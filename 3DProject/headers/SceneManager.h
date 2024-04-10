#pragma once

#include "ShaderD3D11.h"
#include "MeshD3D11.h"
#include "Scene.h"

void CreateScenes(ID3D11Device* device, std::vector<Scene*>& scenes);
