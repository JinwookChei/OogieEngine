#include "stdafx.h"
#include "Level.h"
#include "PlayerCamera.h"
#include "SceneComponent.h"
#include "CameraComponent.h"
#include "RunTimeMode.h"

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
	if (RunTimeMode::GetCurrentMode() != E_RUNTIME_MODE::GAME) return;

	const Float2& deltaMouseMove = InputManager::GetDeltaMouseMove();
	GetComponentTransform().AddRotaionY(deltaMouseMove.Y * deltaTime * 15.0f);

	Float4 scale;
	Float4 quaternion;
	Float4 rotation;
	Float4 position;
	MATH::MatrixDecompose(scale, quaternion, position, GetWorldMatrix());
	MATH::QuaternionToEulerDeg(rotation, quaternion);
	pBindedCamera_->GetWorldTransform().SetRotation(rotation);
	pBindedCamera_->GetWorldTransform().SetPosition(position);
}

void CameraComponent::Render()
{
	SceneComponent::Render();
}

void CameraComponent::CleanUp()
{
}
