#pragma once

#include "headers/CameraD3D11.h"
using namespace DirectX;

void CameraD3D11::MoveInDirection(float amount, const DirectX::XMFLOAT3& direction)
{
	this->position.x += direction.x * amount;
	this->position.y += direction.y * amount;
	this->position.z += direction.z * amount;
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

CameraD3D11::CameraD3D11(ID3D11Device* device, const ProjectionInfo& projectionInfo, const DirectX::XMFLOAT3& initialPosition)
{	
	Initialize(device, projectionInfo, initialPosition);
}

CameraD3D11::CameraD3D11(const CameraD3D11& other)
{
	this->position = other.GetPosition();
	this->forward = other.GetForward();
	this->right = other.GetRight();
	this->up = other.GetUp();
}

void CameraD3D11::Initialize(ID3D11Device* device, const ProjectionInfo& projectionInfo, const DirectX::XMFLOAT3& initialPosition)
{
	this->projInfo = projectionInfo;
	this->position = initialPosition;
	XMFLOAT4X4 vp;
	XMStoreFloat4x4(&vp, XMMatrixMultiply(XMMatrixLookAtLH({initialPosition.x, initialPosition.y, initialPosition.z}, {0, 0, 1}, {0, 1, 0}), XMMatrixPerspectiveFovLH(this->projInfo.fovAngleY, this->projInfo.aspectRatio, this->projInfo.nearZ, this->projInfo.farZ)));
	cameraBuffer.Initialize(device, sizeof(XMFLOAT4X4), &vp);

	XMFLOAT4 pos = XMFLOAT4(initialPosition.x, initialPosition.y, initialPosition.z, 1);
	positionBuffer.Initialize(device, sizeof(XMFLOAT4), &pos);
}

void CameraD3D11::MoveForward(float amount)
{
	this->MoveInDirection(amount, { this->forward.x, 0, this->forward.z });
}

void CameraD3D11::MoveRight(float amount)
{
	this->MoveInDirection(amount, this->right);
}

void CameraD3D11::MoveUp(float amount)
{
	this->MoveInDirection(amount, {0, 1, 0});
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
	this->RotateAroundAxis(amount, {0, 1, 0});
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
	XMFLOAT4X4 viewProjectionFloat = this->GetViewProjectionMatrix();
	cameraBuffer.UpdateBuffer(context, &viewProjectionFloat);


	XMFLOAT4 pos = XMFLOAT4(this->GetPosition().x, this->GetPosition().y, this->GetPosition().z, 1);
	positionBuffer.UpdateBuffer(context, &pos);
}

ID3D11Buffer* CameraD3D11::GetVPBuffer() const
{
	return  cameraBuffer.GetBuffer();
}

ID3D11Buffer* CameraD3D11::GetPositionBuffer() const
{
	return this->positionBuffer.GetBuffer();
}

DirectX::XMFLOAT4X4 CameraD3D11::GetViewProjectionMatrix() const
{
	HXMVECTOR p = XMLoadFloat3(&this->position);
	HXMVECTOR f = XMLoadFloat3(&this->forward);
	HXMVECTOR u = XMLoadFloat3(&this->up);
	XMMATRIX viewMatrix = XMMatrixLookAtLH(p, p+f, u);
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(this->projInfo.fovAngleY, this->projInfo.aspectRatio, this->projInfo.nearZ, this->projInfo.farZ);

	XMMATRIX viewProjectionMatrix = XMMatrixTranspose(XMMatrixMultiply(viewMatrix, projectionMatrix));
	XMFLOAT4X4 viewProjectionFloat;
	XMStoreFloat4x4(&viewProjectionFloat, viewProjectionMatrix);
	return viewProjectionFloat;
}

