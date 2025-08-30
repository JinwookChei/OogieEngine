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
	
	// 축을 정규화
	DirectX::XMVECTOR normAxis = DirectX::XMVector3Normalize(axis);
	float axisLength = DirectX::XMVectorGetX(DirectX::XMVector3Length(normAxis));
	if (axisLength < 1e-6f)
		return; // 유효하지 않은 축이면 회전하지 않음

	// 각도를 라디안으로 변환
	float angleRadians = DirectX::XMConvertToRadians(angleDegrees);

	// 축과 각도로 회전 쿼터니언 생성
	DirectX::XMVECTOR qRotation = DirectX::XMQuaternionRotationAxis(normAxis, angleRadians);

	// 현재 쿼터니언(rotation_) 로드
	DirectX::XMVECTOR currentQuat = XMLoadFloat4(&quaternion_);

	// 새 회전 쿼터니언과 기존 회전 쿼터니언 곱 (새 회전 * 기존 회전)
	DirectX::XMVECTOR newQuat = DirectX::XMQuaternionMultiply(qRotation, currentQuat);

	// 정규화하여 저장
	newQuat = DirectX::XMQuaternionNormalize(newQuat);
	XMStoreFloat4(&quaternion_, newQuat);

	TransformUpdate();
	//// 쿼터니언 기반 회전 행렬 갱신
	//rotationMatrix_ = DirectX::XMMatrixRotationQuaternion(newQuat);

	//// 월드 행렬 갱신 (scale, rotation, translation 순서)
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
	// DirectX는 "라디안(radian)" 단위가 필요하므로, π/180 을 곱하는 벡터를 만듦.
	// 예: 90도 ×(π / 180) = 1.57라디안.
	DirectX::XMFLOAT4 degToRad(PI / 180.0f, PI / 180.0f, PI / 180.0f, 1.0f);

	// XMFLOAT4 rotation -> XMVECTOR 변환.
	// rotation_ (XMFLOAT4)은 (x, y, z, w) 형태의 회전값 저장소.
	// XMLoadFloat4로 XMVECTOR 타입으로 변환 → SIMD 연산 가능.
	DirectX::XMVECTOR rotationVector = DirectX::XMLoadFloat4(&rotation_);

	// rotation을 quaternion으로 변환.(4차원)
	// XMQuaternionRotationRollPitchYawFromVector는(pitch, yaw, roll) → 쿼터니언 변환 함수.
	// radian 변환 전 rotation_을 그대로 사용해서 원본 회전 쿼터니언을 만듦.
	DirectX::XMVECTOR quaternionVectorOrigin = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	// rotation을 radian으로 변환.
	// 라디안 값으로 만든 회전 벡터를 다시 쿼터니언으로 변환.
	// 이게 실제로 월드 행렬에서 사용할 최종 회전값.
	rotationVector = DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&rotation_), DirectX::XMLoadFloat4(&degToRad));

	// radian으로 변환된 rotation에서 quaternion으로 변환.
	DirectX::XMVECTOR quaternionVector = DirectX::XMQuaternionRotationRollPitchYawFromVector(rotationVector);

	DirectX::XMVECTOR scale = DirectX::XMLoadFloat4(&scale_);

	DirectX::XMVECTOR position = DirectX::XMLoadFloat4(&position_);

	// quaternionVectorOrigin 은 회전 기준점(피벗)을 의미, 보통 (0,0,0) 대신 quaternion을 주면 그걸 기준으로 회전.
	// XMMatrixAffineTransformation params.
	//	ㄴ Scaling = Scale Vector {sx, sy, sz, 0}
	//	ㄴ RotaionOrigin(회전 기준점 - Pivot) = {px, py, pz, 1} 예) {0, 0, 0, 1} -> 원점회전, {10, 0, 0, 1} -> {10, 0, 0} 기준 회전. 
	//  ㄴ RotationQuaternion(실제 회전을 표현하는 쿼터니언) 
	//  ㄴ Translation (이동 벡터)
	//worldMatrix_ = DirectX::XMMatrixAffineTransformation(scale, { 0, 0, 0, 1 }, quaternionVector, position);
	worldMatrix_ = DirectX::XMMatrixAffineTransformation(scale, quaternionVectorOrigin, quaternionVector, position);
}
