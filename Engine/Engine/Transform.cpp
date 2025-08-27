#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
	: scale_(1, 1, 1, 0),
	rotation_(0, 0, 0, 0),
	position_(0, 0, 0, 1),
	quaternion_(0, 0, 0, 1),
	scaleMatrix_(DirectX::XMMatrixIdentity()),
	rotationMatrix_(DirectX::XMMatrixIdentity()),
	positionMatrix_(DirectX::XMMatrixIdentity()),
	worldMatrix_(DirectX::XMMatrixIdentity())
{
}

Transform::~Transform()
{
}

void Transform::SetScale(const DirectX::XMFLOAT4& scale)
{
	scale_ = scale;

	TransformUpdate();
}

void Transform::SetRotation(const DirectX::XMFLOAT4& rotation)
{
	rotation_ = rotation;

	TransformUpdate();
}

void Transform::AddRotaionY(float addRotation)
{
	rotation_.y += addRotation;

	TransformUpdate();
}

void Transform::SetPosition(const DirectX::XMFLOAT4& position)
{
	position_ = position;

	TransformUpdate();
}

const DirectX::XMMATRIX Transform::GetWorldMatrix() const
{
	return worldMatrix_;
}

const DirectX::XMMATRIX Transform::GetWorldMatrixTranspose() const
{
	return DirectX::XMMatrixTranspose(worldMatrix_);
}

DirectX::XMVECTOR Transform::GetPosition() const
{
	return DirectX::XMLoadFloat4(&position_);
}

DirectX::XMVECTOR Transform::ForwardVector() const
{
	return DirectX::XMVector3Normalize(worldMatrix_.r[0]);
}

DirectX::XMVECTOR Transform::UpVector() const
{
	return DirectX::XMVector3Normalize(worldMatrix_.r[2]);
}

DirectX::XMVECTOR Transform::RightVector() const
{
	return DirectX::XMVector3Normalize(worldMatrix_.r[1]);
}

void Transform::TransformUpdate()
{
	DirectX::XMFLOAT4 degToRad(3.141592f / 180.0f, 3.141592f / 180.0f, 3.141592f / 180.0f, 1.0f);

	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat4(&rotation_);

	DirectX::XMVECTOR quaternionVectorOrigin = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	rotationVector = DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&rotation_), DirectX::XMLoadFloat4(&degToRad));

	DirectX::XMVECTOR quaternionVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	DirectX::XMVECTOR scale = DirectX::XMLoadFloat4(&scale_);

	DirectX::XMVECTOR position = DirectX::XMLoadFloat4(&position_);

	worldMatrix_ = DirectX::XMMatrixAffineTransformation(scale, quaternionVectorOrigin, quaternionVector, position);
}
