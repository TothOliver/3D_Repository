#include "headers/FrustumCulling.h"


void FrustumCulling::CreateSceneBoundingBox()
{
	BoundingBox sceneBox({ 0, 0, 0 }, { 100, 100, 100 });
	quadTree.SetRoot(sceneBox);

	for (UINT i = 0; i < boundingBoxes.size(); i++) {
		MeshD3D11* elementAdress = objects.at(i);
		quadTree.AddElement(elementAdress, boundingBoxes.at(i));
	}
}

void FrustumCulling::CreateObjectBoundingBox(MeshD3D11*& object)
{
	BoundingBox bb;
	bb.CreateFromPoints(bb, object->GetBoundingBox1(), object->GetBoundingBox2());
	XMFLOAT4 distance = { object->GetWorldPosition().x, object->GetWorldPosition().y, object->GetWorldPosition().z, 1};
	XMFLOAT4 zero({ 0, 0, 0, 1});

	bb.Transform(bb, 1, XMLoadFloat4(&zero), XMLoadFloat4(&distance));
	this->boundingBoxes.push_back(bb);
	this->objects.push_back(object);
}

DirectX::BoundingBox FrustumCulling::GetBoundingBox(UINT index)
{
	return this->boundingBoxes[index];
}

std::vector<const MeshD3D11*> FrustumCulling::GetObjectsArray(CameraD3D11* camera)
{
	XMMATRIX p = camera->GetProjectionMatrix();
	BoundingFrustum frustum;
	BoundingFrustum::CreateFromMatrix(frustum, p);

	XMMATRIX w = camera->GetWorldMatrix();
	frustum.Transform(frustum, w);

	std::vector<const MeshD3D11*> thing = quadTree.CheckTree(frustum);
	return thing;
}

