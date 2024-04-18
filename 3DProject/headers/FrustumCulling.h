#pragma once
#include <DirectXCollision.h>
#include "MeshD3D11.h"
#include "QuadTree.h"

using namespace DirectX;

class FrustumCulling
{
private:
	std::vector<BoundingBox> boundingBoxes;
	std::vector<MeshD3D11*> objects;
	QuadTree<MeshD3D11> quadTree;

public: 
	FrustumCulling() = default;
	~FrustumCulling() = default;
	FrustumCulling(const FrustumCulling& other) = delete;
	FrustumCulling& operator=(const FrustumCulling& other) = delete;
	FrustumCulling(FrustumCulling& other) = delete;
	FrustumCulling& operator=(FrustumCulling&& other) = delete;

	void CreateSceneBoundingBox();
	void CreateObjectBoundingBox(MeshD3D11*& object);

	DirectX::BoundingBox GetBoundingBox(UINT index);
	std::vector<const MeshD3D11*> GetObjectsArray(CameraD3D11* camera);
};

