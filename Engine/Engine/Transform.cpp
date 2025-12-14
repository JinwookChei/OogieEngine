#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
	: scale_(1, 1, 1, 0),
	rotation_(0, 0, 0, 0),
	position_(0, 0, 0, 1),
	quaternion_(0, 0, 0, 1)
{
	MATH::MatrixIdentity(scaleMatrix_);
	MATH::MatrixIdentity(rotationMatrix_);
	MATH::MatrixIdentity(positionMatrix_);
	MATH::MatrixIdentity(worldMatrix_);
}

Transform::~Transform()
{
}

void Transform::SetScale(const Float4& scale)
{
	scale_ = scale;

	TransformUpdate();
}

void Transform::SetRotation(const Float4& rotation)
{
	rotation_ = rotation;

	TransformUpdate();
}

void Transform::AddRotaion(const Float4& offset)
{
	MATH::VectorAdd(rotation_, rotation_, offset);

	TransformUpdate();
}

void Transform::AddRotaionX(float offset)
{
	rotation_.X += offset;

	TransformUpdate();
}

void Transform::AddRotaionY(float offset)
{
	rotation_.Y += offset;

	TransformUpdate();
}

void Transform::AddRotaionZ(float offset)
{
	rotation_.Z += offset;

	TransformUpdate();
}

void Transform::SetPosition(const Float4& position)
{
	position_ = position;

	TransformUpdate();
}

void Transform::AddPosition(const Float4& offset)
{
	MATH::VectorAdd(position_, position_, offset);

	TransformUpdate();
}

void Transform::AddPositionX(float offset)
{
}

void Transform::AddPositionY(float offset)
{
}

void Transform::AddPositionZ(float offset)
{
}

const Float4x4& Transform::GetWorldMatrix() const
{
	return worldMatrix_;
}

const Float4x4 Transform::GetWorldMatrixTranspose() const
{
	Float4x4 ret;
	MATH::MatrixTranspose(ret, worldMatrix_);
	return ret;
}

ENGINE_API const Float4& __stdcall Transform::GetScale() const
{
	return scale_;
}

ENGINE_API const Float4& __stdcall Transform::GetRotation() const
{
	return rotation_;
}

ENGINE_API const Float4& __stdcall Transform::GetQuaternion() const
{
	return quaternion_;
}

const Float4& Transform::GetPosition() const
{
	return position_;
}

Float4 Transform::ForwardVector() const
{
	Float4 normal;
	MATH::VectorNormalize(normal, worldMatrix_.r[0]);
	return normal;
}

Float4 Transform::RightVector() const
{
	Float4 normal;
	MATH::VectorNormalize(normal, worldMatrix_.r[1]);
	return normal;
}

Float4 Transform::UpVector() const
{
	Float4 normal;
	MATH::VectorNormalize(normal, worldMatrix_.r[2]);
	return normal;
}

void Transform::BindOnTransformUpdateLoaded(TransformUpdateDelegate callback)
{
	if (!callback)
	{
		DEBUG_BREAK();
		return;
	}
	OnTransformUpdateLoaded_.push_back(callback);
}

void Transform::BroadcastOnTransformUpdate()
{
	for (const auto& callback : OnTransformUpdateLoaded_)
	{
		if (!callback)
		{
			DEBUG_BREAK();
			continue;
		}

		callback();
	}
}

void Transform::TransformUpdate()
{
	MATH::MatrixCompose(worldMatrix_, scale_, rotation_, position_);

	MATH::MatrixDecompose(scale_, quaternion_, position_, worldMatrix_);

	BroadcastOnTransformUpdate();
}

Float4* __stdcall Transform::GetActorScaleForEditor()
{
	return &scale_;
}

Float4* __stdcall Transform::GetActorRotationForEditor()
{
	return &rotation_;
}

Float4* __stdcall Transform::GetActorQuaternionForEditor()
{
	return &quaternion_;
}

Float4* __stdcall Transform::GetActorPositionForEditor()
{
	return &position_;
}

void __stdcall Transform::TransformUpdateForEditor()
{
	TransformUpdate();
}
