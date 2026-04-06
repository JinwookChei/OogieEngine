#include "stdafx.h"
#include "Transform.h"

Transform::Transform()
	: scale_(1, 1, 1, 0),
	rotation_(0, 0, 0, 0),
	translation_(0, 0, 0, 1),
	quaternion_(0, 0, 0, 1)
{
	MATH::MatrixIdentity(scaleMatrix_);
	MATH::MatrixIdentity(rotationMatrix_);
	MATH::MatrixIdentity(translationMatrix_);
	MATH::MatrixIdentity(affineMatrix_);
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
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
	TransformUpdate();
}

void Transform::AddRotaion(const Float4& offset)
{
	MATH::VectorAdd(rotation_, rotation_, offset);
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
	TransformUpdate();
}

void Transform::AddRotaionX(float offset)
{
	rotation_.X += offset;
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
	TransformUpdate();
}

void Transform::AddRotaionY(float offset)
{
	rotation_.Y += offset;
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
	TransformUpdate();
}

void Transform::AddRotaionZ(float offset)
{
	rotation_.Z += offset;
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
	TransformUpdate();
}

void Transform::SetPosition(const Float4& position)
{
	translation_ = position;
	TransformUpdate();
}

void Transform::AddPosition(const Float4& offset)
{
	MATH::VectorAdd(translation_, translation_, offset);
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

const Float4x4& Transform::GetMatrix() const
{
	return affineMatrix_;
}

const Float4x4 Transform::GetMatrixTranspose() const
{
	Float4x4 ret;
	MATH::MatrixTranspose(ret, affineMatrix_);
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
	return translation_;
}

Float4 Transform::ForwardVector() const
{
	Float4 foward;
	MATH::VectorNormalize(foward, affineMatrix_.r[0]);
	return foward;
}

Float4 Transform::RightVector() const
{
	Float4 right;
	MATH::VectorNormalize(right, affineMatrix_.r[1]);
	return right;
}

Float4 Transform::UpVector() const
{
	Float4 up;
	MATH::VectorNormalize(up, affineMatrix_.r[2]);
	return up;
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
	MATH::MatrixCompose(affineMatrix_, scaleMatrix_, rotationMatrix_, translationMatrix_, scale_, quaternion_, translation_);
	BroadcastOnTransformUpdate();
}

void Transform::TransformUpdateTTT()
{
	MATH::MatrixDecompose(scale_, quaternion_, translation_, affineMatrix_);
	MATH::QuaternionToEulerDeg(rotation_, quaternion_);
	TransformUpdate();
}

const Float4x4& __stdcall Transform::GetMatrixForEditor() const
{
	return affineMatrix_;
}

Float4& __stdcall Transform::GetActorScaleForEditor()
{
	return scale_;
}

Float4& __stdcall Transform::GetActorRotationForEditor()
{
	return rotation_;
}

Float4& __stdcall Transform::GetActorQuaternionForEditor()
{
	return quaternion_;
}

Float4& __stdcall Transform::GetActorPositionForEditor()
{
	return translation_;
}

void __stdcall Transform::SetActorScaleForEditor(const Float4& scale)
{
	scale_ = scale;
}

void __stdcall Transform::SetActorRotationForEditor(const Float4& rot)
{
	rotation_ = rot;
	MATH::EulerDegToQuaternion(quaternion_, rotation_);
}

void __stdcall Transform::SetActorPositionForEditor(const Float4& pos)
{
	translation_ = pos;
}

void __stdcall Transform::TransformUpdateForEditor()
{
	TransformUpdate();
}

void __stdcall Transform::TransformUpdateForEditorTTT()
{
	TransformUpdateTTT();
}
