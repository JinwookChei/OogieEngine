#include "stdafx.h"
#include "Level.h"
#include "PlayerCamera.h"
#include "SceneComponent.h"
#include "CameraComponent.h"

CameraComponent::CameraComponent()
	: pBindedCamera_(nullptr)
{
}

CameraComponent::~CameraComponent()
{
	CleanUp();
}

void CameraComponent::BeginPlay()
{
	SceneComponent::BeginPlay();
	pBindedCamera_ = World::Instance()->GetLevel()->SpawnCamera<PlayerCamera>();
	pBindedCamera_->BindComponent(this);
	CameraManager::SetPlayerCamera(pBindedCamera_);
}

void CameraComponent::Tick(double deltaTime)
{
	SceneComponent::Tick(deltaTime);

	GetWorldMatrix();
	Float4 scale;
	Float4 quaternion;
	Float4 rotation;
	Float4 position;
	MATH::MatrixDecompose(scale, quaternion, position, GetWorldMatrix());
	MATH::QuaternionToEulerDeg(rotation, quaternion);
	pBindedCamera_->GetWorldTransform().SetPosition(position);
	pBindedCamera_->GetWorldTransform().SetRotation(rotation);
}

void CameraComponent::Render()
{
	SceneComponent::Render();
}

void CameraComponent::CleanUp()
{
}
