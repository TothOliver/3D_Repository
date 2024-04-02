#include "headers/SceneManager.h"

Scene::~Scene()
{
	for (size_t i = 0; i < this->objectsInScene.size(); i++)
	{
		this->objectsInScene.at(i)->~MeshD3D11();
		this->objectsInScene.at(i) = nullptr;
	}
}

void Scene::DrawScene(ID3D11DeviceContext*& immediateContext)
//Draw all objects in the scene
{
	for (size_t i = 0; i < this->objectsInScene.size(); i++)
	{
		this->objectsInScene.at(i)->BindMeshBuffers(immediateContext);
	}
}

void Scene::AddObject(MeshD3D11*& object)
{
	this->objectsInScene.push_back(object);
}


int CreateScenes(ID3D11Device* device, std::vector<Scene*>& scenes)
{
	//This is where created and stored into the vector which will be used by the draw loop to decide what scene to render
	//Most scenes will most likely only contain one object
	//Scene* scene2 = new Scene();

	//MeshD3D11* rat = new MeshD3D11(device, "rat.obj", 0, -2, -1);
	//scene2->AddObject(rat);
	//MeshD3D11* teapot = new MeshD3D11(device, "teapot.obj");
	//scene2->AddObject(teapot);

	Scene* scene1 = new Scene();

	MeshD3D11* rat1 = new MeshD3D11(device, "rat.obj", 0, 0, 3);
	MeshD3D11* rat2 = new MeshD3D11(device, "rat.obj", 0, 0, 0);
	MeshD3D11* rat3 = new MeshD3D11(device, "rat.obj", 0, 0, -3);
	MeshD3D11* rat4 = new MeshD3D11(device, "rat.obj", 0, -2, 0);

	MeshD3D11* light1 = new MeshD3D11(device, "bulb.obj", 0, 0.5, 1);
	MeshD3D11* light2 = new MeshD3D11(device, "bulb.obj", 0, 1, -1);
	MeshD3D11* light3 = new MeshD3D11(device, "bulb.obj", 0, -1, -3);
	MeshD3D11* light4 = new MeshD3D11(device, "bulb.obj", 0, 1, -2);

	scene1->AddObject(rat1);
	scene1->AddObject(rat2);
	scene1->AddObject(rat3);
	scene1->AddObject(rat4);
	scene1->AddObject(light1);
	scene1->AddObject(light2);
	scene1->AddObject(light3);
	scene1->AddObject(light4);


	//Scene* scene3 = new Scene();
	//MeshD3D11* axe = new MeshD3D11(device, "axe.obj");
	//scene3->AddObject(axe);

	scenes.push_back(scene1);
	//scenes.push_back(scene2);
	//scenes.push_back(scene3);

	//Scene* scene4 = new Scene();

	//MeshD3D11* plane = new MeshD3D11(device, "plane.obj", 0, -5, 0);
	//MeshD3D11* sphere = new MeshD3D11(device, "sphere.obj");
	//scene4->AddObject(plane);
	//scene4->AddObject(sphere);
	//scenes.push_back(scene4);


	return 0;
}