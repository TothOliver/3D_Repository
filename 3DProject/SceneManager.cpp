#include "headers/SceneManager.h"


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

	MeshD3D11* foam = new MeshD3D11(device, "foam.obj", 0, 0, 10, 1);
	scene1->AddObject(foam);

	MeshD3D11* stone = new MeshD3D11(device, "stoneWall.obj", 0, 0, -10, 1, true);
	scene1->AddObject(stone);

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

	MeshD3D11* sphere = new MeshD3D11(device, "sphere.obj");
	scene4->AddObject(sphere);
	scenes.push_back(scene4);

	LightData light0;
	LightData light1;
	LightData light2;
	LightData light3;

	light0.initialPosition = { 0, 10, 0 , 0 };
	light0.intensity = { 0.4, 0.4, 1, 0 };
	light0.rotationX = 0;
	light0.rotationY = -DirectX::XM_PIDIV2;
	light0.directionalLight = true;
	light0.nearZ = 0.1f;
	light0.farZ = 100.0f;
	light0.angle = 1;

	light1.intensity = { 0.3, 10, 2, 0 };
	light1.rotationY = -DirectX::XM_PIDIV2;
	light1.rotationX = 0;
	light1.angle = 1;
	light1.initialPosition = { 0, 8, 0, 0 };
	light1.nearZ = 0.01;
	light1.farZ = 100;

	light2.intensity = { 0.7, 1.4, 1.5, 0 };
	light2.rotationX = DirectX::XM_PI;
	light2.rotationY = 0;
	light2.angle = 0.5;
	light2.initialPosition = { 0, 1, -1, 0 };

	light3.intensity = { 0.7, 1, 0.5, 0 };
	light3.rotationX = 0;
	light3.rotationY = DirectX::XM_PIDIV2;
	light3.angle = 2;
	light3.initialPosition = { 0, 1, 0, 0 };

	scene1->AddLight(device, light1);
	scene1->AddLight(device, light2);
	scene1->AddLight(device, light3);

	for (size_t i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->UpdateNrOfLights(device);
		scenes.at(i)->InitializeStructuredBuffer(device);
	}
}