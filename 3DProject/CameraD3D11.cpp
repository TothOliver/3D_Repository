#include "CameraD3D11.h"
using namespace DirectX;

void CameraD3D11::MoveInDirection(float amount, const DirectX::XMFLOAT3& direction)
{
	this->position.x = direction.x* amount;
	this->position.y = direction.y * amount;
	this->position.z = direction.z * amount;
	this->position = { this->position.x, this->position.y, this->position.z };
}

void CameraD3D11::RotateAroundAxis(float amount, const DirectX::XMFLOAT3& axis)
{
	XMMATRIX rotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&axis), amount);

	XMVECTOR newForward = XMVector3TransformNormal(XMLoadFloat3(&this->forward), rotationMatrix);
	XMVECTOR newRight = XMVector3TransformNormal(XMLoadFloat3(&this->right), rotationMatrix);
	XMVECTOR newUp = XMVector3TransformNormal(XMLoadFloat3(&this->up), rotationMatrix);

	XMStoreFloat3(&this->forward, newForward);
	XMStoreFloat3(&this->right, newRight);
	XMStoreFloat3(&this->up, newUp);
}

CameraD3D11::CameraD3D11(ID3D11Device* device, const ProjectionInfo& projectionInfo,
	const DirectX::XMFLOAT3& initialPosition)
{	
	Initialize(device, projectionInfo, initialPosition);
}

void CameraD3D11::Initialize(ID3D11Device* device, const ProjectionInfo& projectionInfo, const DirectX::XMFLOAT3& initialPosition)
{
	this->projInfo = projectionInfo;
	this->position = initialPosition;
	cameraBuffer.Initialize(device, sizeof(CameraD3D11), &this->position);
}

void CameraD3D11::MoveForward(float amount)
{
	this->MoveInDirection(amount, { 0.0f, 0.0f, 1.0f });
}

void CameraD3D11::MoveRight(float amount)
{
	this->MoveInDirection(amount, { 1.0f, 0.0f, 0.0f });
}

void CameraD3D11::MoveUp(float amount)
{
	this->MoveInDirection(amount, { 0.0f, 1.0f, 0.0f });
}

void CameraD3D11::RotateForward(float amount)
{
	this->RotateAroundAxis(amount, this->forward);
}

void CameraD3D11::RotateRight(float amount)
{
	this->RotateAroundAxis(amount, this->right);
}

void CameraD3D11::RotateUp(float amount)
{
	this->RotateAroundAxis(amount, this->up);
}

const DirectX::XMFLOAT3& CameraD3D11::GetPosition() const
{
	return this->position;
}

const DirectX::XMFLOAT3& CameraD3D11::GetForward() const
{
	return this->forward;
}

const DirectX::XMFLOAT3& CameraD3D11::GetRight() const
{
	return this->right;
}

const DirectX::XMFLOAT3& CameraD3D11::GetUp() const
{
	return this->up;
}

void CameraD3D11::UpdateInternalConstantBuffer(ID3D11DeviceContext* context)
{
	XMFLOAT4X4 viewProjectionMatrix = this->GetViewProjectionMatrix();
	cameraBuffer.UpdateBuffer(context, &viewProjectionMatrix);
}

ID3D11Buffer* CameraD3D11::GetConstantBuffer() const
{
	return  cameraBuffer.GetBuffer();
}

DirectX::XMFLOAT4X4 CameraD3D11::GetViewProjectionMatrix() const
{
	ProjectionInfo pi;
	HXMVECTOR position = XMLoadFloat3(&this->position);
	HXMVECTOR forward = XMLoadFloat3(&this->forward);
	HXMVECTOR up = XMLoadFloat3(&this->up);
	XMMATRIX viewMatrix = XMMatrixLookAtLH(position, forward, up);
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(pi.fovAngleY, pi.aspectRatio, pi.nearZ, pi.farZ);

	XMMATRIX viewProjectionMatrix =(XMMatrixMultiply(viewMatrix, projectionMatrix));
	XMFLOAT4X4 viewProjectionFloat;
	XMStoreFloat4x4(&viewProjectionFloat, viewProjectionMatrix);
	return viewProjectionFloat;
}

