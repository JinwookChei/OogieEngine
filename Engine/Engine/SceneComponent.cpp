#include "stdafx.h"
#include "ActorComponent.h"
#include "SceneComponent.h"

SceneComponent::SceneComponent()
	: pTransform_(new Transform)
{
	pTransform_;
}

SceneComponent::~SceneComponent()
{
	CleanUp();
}

void SceneComponent::BeginPlay()
{
}

void SceneComponent::Tick(double deltaTime)
{
	ActorComponent::Tick(deltaTime);
}

void SceneComponent::Render()
{
	ActorComponent::Render();
}

Transform& SceneComponent::GetComponentTransform() const
{
	return *pTransform_;
}

Float4x4 SceneComponent::GetWorldMatrix() const
{
	Float4x4 worldMat;
	MATH::MatrixMultiply(worldMat, GetOwner()->GetWorldTransform().GetMatrix(), pTransform_->GetMatrix());
	return worldMat;
}

Float4 SceneComponent::GetWorldScale() const
{
	Float4x4 worldMat;
	MATH::MatrixMultiply(worldMat, GetOwner()->GetWorldTransform().GetMatrix(), pTransform_->GetMatrix());
	Float4 scale;
	Float4 quat;
	Float4 pos;
	MATH::MatrixDecompose(scale, quat, pos, worldMat);
	return scale;
}

Float4 SceneComponent::GetWorldRotation() const
{
	Float4x4 worldMat;
	MATH::MatrixMultiply(worldMat, GetOwner()->GetWorldTransform().GetMatrix(), pTransform_->GetMatrix());
	Float4 scale;
	Float4 quat;
	Float4 degree;
	Float4 pos;
	MATH::MatrixDecompose(scale, quat, pos, worldMat);
	MATH::QuaternionToEulerDeg(degree, quat);
	return degree;
}

Float4 SceneComponent::GetWorldPosition() const
{
	Float4x4 worldMat;
	MATH::MatrixMultiply(worldMat, GetOwner()->GetWorldTransform().GetMatrix(), pTransform_->GetMatrix());
	Float4 scale;
	Float4 quat;
	Float4 pos;
	MATH::MatrixDecompose(scale, quat, pos, worldMat);
	return pos;
}

Float4 SceneComponent::GetLocalScale() const
{
	return pTransform_->GetScale();
}

Float4 SceneComponent::GetLocalRotation() const
{
	return pTransform_->GetRotation();
}

Float4 SceneComponent::GetLocalPosition() const
{
	return pTransform_->GetPosition();
}

void SceneComponent::CleanUp()
{
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
}