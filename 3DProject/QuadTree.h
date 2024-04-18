#pragma once

#include <algorithm>

using namespace DirectX;

template <typename T>
class QuadTree
{
private:
	struct Node
	{
		BoundingBox nodeBox;
		std::vector<const T*> elements;
		std::unique_ptr<Node> children[4];
	};
	std::unique_ptr<Node> root;
	std::vector<BoundingBox*> sceneBoundingBox;

	void PrintTree(std::unique_ptr<Node>& nodeToProcess);
	bool AddToNode(const T* elementAddress, const DirectX::BoundingBox& objectboundingBox, std::unique_ptr<Node>& node);
	void SubDividedNode(std::unique_ptr<Node>& node);
	void CheckNode(const DirectX::BoundingFrustum& frustum, const std::unique_ptr<Node>& node, std::vector<const T*>& foundObjects);

public:
	QuadTree() = default;
	~QuadTree() = default;
	QuadTree(const QuadTree& other) = delete;
	QuadTree& operator=(const QuadTree& other) = delete;
	QuadTree(QuadTree& other) = delete;
	QuadTree& operator=(QuadTree&& other) = delete;

	void SetRoot(const DirectX::BoundingBox sceneBox);
	void PrintTree();
	void AddElement(const T* elementAddress, const DirectX::BoundingBox& boundingBox);
	std::vector<const T*> CheckTree(const DirectX::BoundingFrustum& frustum);
};

template<typename T>
void QuadTree<T>::SetRoot(DirectX::BoundingBox sceneBox)
{
	this->root = std::make_unique<Node>();
	this->root->nodeBox = sceneBox;
}

template <typename T>
void QuadTree<T>::PrintTree()
{
	PrintTree(root);
}

template <typename T>
void QuadTree<T>::PrintTree(std::unique_ptr<Node>& nodeToProcess)
{
	// If all nodes, parent and child, contain information to be printed then do that here

	if (nodeToProcess->children[0] == nullptr) {
		//print contained information
	}
	else {
		for (UINT i = 0; i < 4; i++) {
			if (nodeToProcess->children[i] != nullptr) {}
			//PrintTree(nodeToProcess->children[i]);
		}
	}
}

template<typename T>
inline void QuadTree<T>::SubDividedNode(std::unique_ptr<Node>& node)
{
	float childWidth = node->nodeBox.Extents.x / 2;
	float childHeight = node->nodeBox.Extents.z / 2;
	DirectX::XMFLOAT3 center = node->nodeBox.Center;

	node->children[0] = std::make_unique<Node>();
	node->children[1] = std::make_unique<Node>();
	node->children[2] = std::make_unique<Node>();
	node->children[3] = std::make_unique<Node>();

	node->children[0]->nodeBox = DirectX::BoundingBox({ center.x + childWidth, 0, center.z + childWidth }, { childWidth, 100, childHeight }); //up-right
	node->children[1]->nodeBox = DirectX::BoundingBox({ center.x - childWidth, 0, center.z + childWidth }, { childWidth, 100, childHeight }); //up-left
	node->children[2]->nodeBox = DirectX::BoundingBox({ center.x - childWidth, 0, center.z - childWidth }, { childWidth, 100, childHeight }); //down-left
	node->children[3]->nodeBox = DirectX::BoundingBox({ center.x + childWidth, 0, center.z - childWidth }, { childWidth, 100, childHeight }); //down-right

	for (UINT i = 0; i < node->elements.size(); i++) {
		for (UINT j = 0; j < 4; j++) {
			DirectX::BoundingBox b = node->elements.at(i)->GetBoundingBox();
			if (node->children[j]->nodeBox.Intersects(b)) {
				node->children[j]->elements.push_back(node->elements.at(i));
				break;
			}
		}
	}

	node->elements.clear();
}

template <typename T>
void QuadTree<T>::AddElement(const T* elementAddress, const DirectX::BoundingBox& boundingBox)
{
	AddToNode(elementAddress, boundingBox, root);
}

template <typename T>
bool QuadTree<T>::AddToNode(const T* elementAddress, const DirectX::BoundingBox& objectboundingBox, std::unique_ptr<Node>& node)
{
	bool collision = node->nodeBox.Intersects(objectboundingBox);

	if (!collision)
		return false;

	if (!node->children[0])
	{
		if (node->elements.size() < 4)
		{
			node->elements.push_back(elementAddress);
			return true;
		}
		else
			SubDividedNode(node);

	}

	for (auto& child : node->children)
		if (AddToNode(elementAddress, objectboundingBox, child))
			break;

}

template<typename T>
std::vector<const T*> QuadTree<T>::CheckTree(const DirectX::BoundingFrustum& frustum)
{
	std::vector<const T*> toReturn;
	CheckNode(frustum, root, toReturn);

	return toReturn;
}

template < typename T>
void QuadTree<T>::CheckNode(const DirectX::BoundingFrustum& frustum, const std::unique_ptr<Node>& node, std::vector<const T*>& foundObjects)
{
	bool collision = frustum.Intersects(node->nodeBox);

	if (!collision)
		return;

	if (!node->children[0])
	{
		for (UINT i = 0; i < node->elements.size(); i++)
		{
			collision = frustum.Intersects(node->nodeBox);

			if (collision)
			{
				//if (std::find(foundObjects.begin(), foundObjects.end(), node->elements.at(i)) != foundObjects.end())
				foundObjects.push_back(node->elements.at(i));
			}
		}
	}
	else
	{
		for (auto& child : node->children)
			CheckNode(frustum, child, foundObjects);
	}
}