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

void Transform::SetPosition(const DirectX::XMFLOAT4& position)
{
	position_ = position;

	TransformUpdate();
}

void Transform::AddRotaionRoll(float roll)
{
	rotation_.x += roll;

	TransformUpdate();
}

void Transform::AddRotaionPitch(float pitch)
{
	rotation_.y += pitch;

	TransformUpdate();
}

void Transform::AddRotaionYaw(float yaw)
{
	rotation_.z += yaw;

	TransformUpdate();
}

void Transform::AddPosition(const Vector& add)
{
	position_.x += add.X;
	position_.y += add.Y;
	position_.z += add.Z;

	TransformUpdate();
}


void Transform::AddPositionX(float x)
{
	position_.x += x;

	TransformUpdate();
}

void Transform::AddPositionY(float y)
{
	position_.y += y;

	TransformUpdate();
}

void Transform::AddPositionZ(float z)
{
	position_.z += z;

	TransformUpdate();
}

void Transform::RotateAroundAxis(const DirectX::XMVECTOR& axis, float angleDegrees){
	
	// ���� ����ȭ
	DirectX::XMVECTOR normAxis = DirectX::XMVector3Normalize(axis);
	float axisLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(normAxis));
	if (axisLength < 1e-6f)
		return; // ��ȿ���� ���� ���̸� ȸ������ ����

	// ������ �������� ��ȯ
	float angleRadians = DirectX::XMConvertToRadians(angleDegrees);

	// ��� ������ ȸ�� ���ʹϾ� ����
	DirectX::XMVECTOR qRotation = DirectX::XMQuaternionRotationAxis(normAxis, angleRadians);

	// ���� ���ʹϾ�(rotation_) �ε�
	DirectX::XMVECTOR currentQuat = XMLoadFloat4(&quaternion_);

	// �� ȸ�� ���ʹϾ�� ���� ȸ�� ���ʹϾ� �� (�� ȸ�� * ���� ȸ��)
	DirectX::XMVECTOR newQuat = DirectX::XMQuaternionMultiply(qRotation, currentQuat);

	// ����ȭ�Ͽ� ����
	newQuat = DirectX::XMQuaternionNormalize(newQuat);
	XMStoreFloat4(&quaternion_, newQuat);

	TransformUpdate();
	//// ���ʹϾ� ��� ȸ�� ��� ����
	//rotationMatrix_ = DirectX::XMMatrixRotationQuaternion(newQuat);

	//// ���� ��� ���� (scale, rotation, translation ����)
	//scaleMatrix_ = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	//positionMatrix_ = DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);
	//worldMatrix_ = scaleMatrix_ * rotationMatrix_ * positionMatrix_;
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

DirectX::XMVECTOR Transform::RightVector() const
{
	return DirectX::XMVector3Normalize(worldMatrix_.r[1]);
}

DirectX::XMVECTOR Transform::UpVector() const
{
	return DirectX::XMVector3Normalize(worldMatrix_.r[2]);
}

void Transform::TransformUpdate()
{
	// degree -> radian
	// DirectX�� "����(radian)" ������ �ʿ��ϹǷ�, ��/180 �� ���ϴ� ���͸� ����.
	// ��: 90�� ��(�� / 180) = 1.57����.
	DirectX::XMFLOAT4 degToRad(PI / 180.0f, PI / 180.0f, PI / 180.0f, 1.0f);

	// XMFLOAT4 rotation -> XMVECTOR ��ȯ.
	// rotation_ (XMFLOAT4)�� (x, y, z, w) ������ ȸ���� �����.
	// XMLoadFloat4�� XMVECTOR Ÿ������ ��ȯ �� SIMD ���� ����.
	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat4(&rotation_);

	// rotation�� quaternion���� ��ȯ.(4����)
	// XMQuaternionRotationRollPitchYawFromVector��(pitch, yaw, roll) �� ���ʹϾ� ��ȯ �Լ�.
	// radian ��ȯ �� rotation_�� �״�� ����ؼ� ���� ȸ�� ���ʹϾ��� ����.
	DirectX::XMVECTOR quaternionVectorOrigin = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	// rotation�� radian���� ��ȯ.
	// ���� ������ ���� ȸ�� ���͸� �ٽ� ���ʹϾ����� ��ȯ.
	// �̰� ������ ���� ��Ŀ��� ����� ���� ȸ����.
	rotationVector = DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&rotation_), DirectX::XMLoadFloat4(&degToRad));

	// radian���� ��ȯ�� rotation���� quaternion���� ��ȯ.
	DirectX::XMVECTOR quaternionVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	DirectX::XMVECTOR scale = DirectX::XMLoadFloat4(&scale_);

	DirectX::XMVECTOR position = DirectX::XMLoadFloat4(&position_);

	// quaternionVectorOrigin �� ȸ�� ������(�ǹ�)�� �ǹ�, ���� (0,0,0) ��� quaternion�� �ָ� �װ� �������� ȸ��.
	// XMMatrixAffineTransformation params.
	//	�� Scaling = Scale Vector {sx, sy, sz, 0}
	//	�� RotaionOrigin(ȸ�� ������ - Pivot) = {px, py, pz, 1} ��) {0, 0, 0, 1} -> ����ȸ��, {10, 0, 0, 1} -> {10, 0, 0} ���� ȸ��. 
	//  �� RotationQuaternion(���� ȸ���� ǥ���ϴ� ���ʹϾ�) 
	//  �� Translation (�̵� ����)
	//worldMatrix_ = DirectX::XMMatrixAffineTransformation(scale, { 0, 0, 0, 1 }, quaternionVector, position);
	worldMatrix_ = DirectX::XMMatrixAffineTransformation(scale, quaternionVectorOrigin, quaternionVector, position);
}
