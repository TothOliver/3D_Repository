#include "headers/SceneManager.h"

void CreateScenes(ID3D11Device* device, std::vector<Scene*>& scenes)
{
	//This is where created and stored into the vector which will be used by the draw loop to decide what scene to render
	//Most scenes will most likely only contain one object

	Scene* scene1 = new Scene();

	MeshD3D11* foam = new MeshD3D11(device, "foam.obj", 0, 0, 10, 1, false);
	scene1->AddObject(foam);

	MeshD3D11* stone = new MeshD3D11(device, "foam.obj", 0, 0, -10, 1, true);
	scene1->AddObject(stone);

	Scene* scene2 = new Scene();

	/*MeshD3D11* rat = new MeshD3D11(device, "rat.obj", 10, 0, 0);
	MeshD3D11* rat5 = new MeshD3D11(device, "rat.obj", -10, 0, 0);
	MeshD3D11* rat55 = new MeshD3D11(device, "rat.obj", 0, 10, 0);
	MeshD3D11* rat555 = new MeshD3D11(device, "rat.obj", 0, -10, 0);
	MeshD3D11* rat5555 = new MeshD3D11(device, "rat.obj", 0, 0, 10);
	MeshD3D11* rat55555 = new MeshD3D11(device, "rat.obj", 0, 0, -10);

	scene2->AddObject(rat);
	scene2->AddObject(rat5);
	scene2->AddObject(rat55);
	scene2->AddObject(rat555);
	scene2->AddObject(rat5555);
	scene2->AddObject(rat55555);*/

	MeshD3D11* sphere = new MeshD3D11(device, "sphere.obj", 0, 0, 0, 1, false, true);
	scene2->AddObject(sphere);

	MeshD3D11* invCube = new MeshD3D11(device, "invertedCube.obj", 0, 0, 0, 20);
	scene2->AddObject(invCube);

	Scene* scene3 = new Scene();

	MeshD3D11* rat1 = new MeshD3D11(device, "rat.obj", 0, 0, 0);
	MeshD3D11* rat2 = new MeshD3D11(device, "rat.obj", 5, 0, 0);
	MeshD3D11* rat3 = new MeshD3D11(device, "rat.obj", 10, 0, 0);
	MeshD3D11* rat4 = new MeshD3D11(device, "rat.obj", 15, 0, 0);
	MeshD3D11* rat5 = new MeshD3D11(device, "rat.obj", -5, 0, 0);
	MeshD3D11* rat6 = new MeshD3D11(device, "rat.obj", -10, 0, 0);

	MeshD3D11* rat7 = new MeshD3D11(device, "rat.obj", -15, 0, 0);
	MeshD3D11* rat8 = new MeshD3D11(device, "rat.obj", 0, 0, 5);
	MeshD3D11* rat9 = new MeshD3D11(device, "rat.obj", 0, 0, 10);
	MeshD3D11* rat10 = new MeshD3D11(device, "rat.obj", 0, 0, 15);
	MeshD3D11* rat11 = new MeshD3D11(device, "rat.obj", 0, 0, -5);
	MeshD3D11* rat12 = new MeshD3D11(device, "rat.obj", 0, 0, -10);

	MeshD3D11* rat13 = new MeshD3D11(device, "rat.obj", 0, 0, -15);
	MeshD3D11* rat14 = new MeshD3D11(device, "rat.obj", 5, 0, 5);
	MeshD3D11* rat15 = new MeshD3D11(device, "rat.obj", -5, 0, -5);
	MeshD3D11* rat16 = new MeshD3D11(device, "rat.obj", 5, 0, -5);
	MeshD3D11* rat17 = new MeshD3D11(device, "rat.obj", -5, 0, 5);
	MeshD3D11* rat18 = new MeshD3D11(device, "rat.obj", 10, 0, 10);

	MeshD3D11* rat19 = new MeshD3D11(device, "rat.obj", -10, 0, -10);
	MeshD3D11* rat20 = new MeshD3D11(device, "rat.obj", 10, 0, -10);
	MeshD3D11* rat21 = new MeshD3D11(device, "rat.obj", -10, 0, 10);


	scene3->AddObject(rat1);
	scene3->AddObject(rat2);
	scene3->AddObject(rat3);
	scene3->AddObject(rat4);
	scene3->AddObject(rat5);
	scene3->AddObject(rat6);
	scene3->AddObject(rat7);
	scene3->AddObject(rat8);
	scene3->AddObject(rat9);
	scene3->AddObject(rat10);
	scene3->AddObject(rat11);
	scene3->AddObject(rat12);
	scene3->AddObject(rat13);
	scene3->AddObject(rat14);
	scene3->AddObject(rat15);
	scene3->AddObject(rat16);
	scene3->AddObject(rat17);
	scene3->AddObject(rat18);
	scene3->AddObject(rat19);
	scene3->AddObject(rat20);
	scene3->AddObject(rat21);


	scenes.push_back(scene1);
	scenes.push_back(scene2);
	scenes.push_back(scene3);


	LightData light0;
	LightData light1;
	LightData light2;
	LightData light3;
	LightData light4;

	light0.initialPosition = { 0, 10, 0 , 0 };
	light0.intensity = { 0.7, 0.8, 1, 0 };
	light0.rotationX = 0;
	light0.rotationY = -DirectX::XM_PIDIV2;
	light0.directionalLight = true;
	light0.nearZ = 0.1f;
	light0.farZ = 100.0f;
	light0.angle = 1;

	light1.intensity = { 0.3, 1, 2, 0 };
	light1.rotationY = 0;
	light1.rotationX = -DirectX::XM_PIDIV2;
	light1.angle = 1;
	light1.initialPosition = { 3, 1, 2, 0 };
	light1.nearZ = 0.01;
	light1.farZ = 100;

	scene1->AddLight(device, light0);

	scene2->AddLight(device, light0);

	scene3->AddLight(device, light0);
	scene3->AddLight(device, light1);

	scene1->InitializeParticleEmitter(device, 1000, DirectX::XMFLOAT3(0, 2, 0), DirectX::XM_PIDIV2 / 4, 10, 0.003, SMOKE);
	scene1->InitializeParticleEmitter(device, 1000, DirectX::XMFLOAT3(0, 0, 0), 0, 20, 0.05, RAIN);


	for (size_t i = 0; i < scenes.size(); i++)
	{
		scenes.at(i)->UpdateNrOfLights(device);
		scenes.at(i)->InitializeStructuredBuffer(device);
		scenes.at(i)->CreateQuadTree();
	}
}