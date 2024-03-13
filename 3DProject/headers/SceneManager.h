#pragma once
#include "MeshD3D11.h"



class Scene
{
private:
	std::vector<MeshD3D11*> objectsInScene;

public:
	Scene() = default;
	~Scene();
	void DrawScene(ID3D11DeviceContext*& immediateContext);
	void AddObject(MeshD3D11*& object);
};

int CreateScenes(ID3D11Device* device, std::vector<Scene>& scenes);