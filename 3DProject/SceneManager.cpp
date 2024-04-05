#include "headers/SceneManager.h"

Scene::~Scene()
{
	for (size_t i = 0; i < this->objectsInScene.size(); i++)
	{
		this->objectsInScene.at(i)->~MeshD3D11();
		this->objectsInScene.at(i) = nullptr;
	}
}

void Scene::DrawScene(ID3D11DeviceContext*& immediateContext, ShaderD3D11 ps[2])
//Draw all objects in the scene
{
	for (size_t i = 0; i < this->objectsInScene.size(); i++)
	{
		ID3D11ShaderResourceView* srv;
		if (this->objectsInScene.at(i)->IsReflective())
		{
			ps[1].BindShader(immediateContext);
			srv = this->objectsInScene.at(i)->GetTextureCube()->GetCubeSRV();
			immediateContext->PSSetShaderResources(6, 1, &srv);
		}
		else
			ps[0].BindShader(immediateContext);

		this->objectsInScene.at(i)->BindMeshBuffers(immediateContext);
		srv = nullptr;
		immediateContext->PSSetShaderResources(6, 1, &srv);
	}
}

void Scene::AddObject(MeshD3D11*& object)
{
	this->objectsInScene.push_back(object);
}

int Scene::GetNrOfMeshes() const
{
	return this->objectsInScene.size();
}

MeshD3D11* Scene::GetMeshAt(int index) const
{ 
	return this->objectsInScene.at(index);
}

void CreateScenes(ID3D11Device* device, std::vector<Scene*>& scenes)
{
	//This is where created and stored into the vector which will be used by the draw loop to decide what scene to render
	//Most scenes will most likely only contain one object
	Scene* scene1 = new Scene();

	MeshD3D11* rat = new MeshD3D11(device, "rat.obj", 10, 0, 0);
	MeshD3D11* rat5 = new MeshD3D11(device, "rat.obj", -10, 0, 0);
	MeshD3D11* rat55 = new MeshD3D11(device, "rat.obj", 0, 10, 0);
	MeshD3D11* rat555 = new MeshD3D11(device, "rat.obj", 0, -10, 0);
	MeshD3D11* rat5555 = new MeshD3D11(device, "rat.obj", 0, 0, 10);
	MeshD3D11* rat55555 = new MeshD3D11(device, "rat.obj", 0, 0, -10);

	scene1->AddObject(rat);
	scene1->AddObject(rat5);
	scene1->AddObject(rat55);
	scene1->AddObject(rat555);
	scene1->AddObject(rat5555);
	scene1->AddObject(rat55555);

	MeshD3D11* reflective = new MeshD3D11(device, "cube.obj", 0, 0, 0, 1);
	MeshD3D11* floor = new MeshD3D11(device, "plane.obj", 0, -1, 0, 1, true, 512);
	scene1->AddObject(reflective);
	scene1->AddObject(floor);

	Scene* scene2 = new Scene();

	MeshD3D11* rat1 = new MeshD3D11(device, "rat.obj", -5, 0, 0);
	MeshD3D11* rat2 = new MeshD3D11(device, "rat.obj", 0, 0, 0);
	MeshD3D11* rat3 = new MeshD3D11(device, "rat.obj", 5, 0, 0);

	scene2->AddObject(rat1);
	scene2->AddObject(rat2);
	scene2->AddObject(rat3);

	Scene* scene3 = new Scene();
	MeshD3D11* axe = new MeshD3D11(device, "axe.obj", 0, 0, -2, 1);
	scene3->AddObject(axe);

	scenes.push_back(scene1);
	scenes.push_back(scene2);
	scenes.push_back(scene3);

	Scene* scene4 = new Scene();

	MeshD3D11* plane = new MeshD3D11(device, "plane.obj", 0, -5, 0);
	MeshD3D11* sphere = new MeshD3D11(device, "sphere.obj");
	scene4->AddObject(plane);
	scene4->AddObject(sphere);
	scenes.push_back(scene4);
}