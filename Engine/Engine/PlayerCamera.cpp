#include "stdafx.h"
#include "Camera.h"
#include "CameraComponent.h"
#include "PlayerCamera.h"

PlayerCamera::PlayerCamera()
	: pOwnerComponent_(nullptr)
{
}

PlayerCamera::~PlayerCamera()
{
	CleanUp();
}

void PlayerCamera::Tick(double deltaTime)
{
	Camera::Tick(deltaTime);
}

void PlayerCamera::BeginPlay()
{
	Camera::BeginPlay();
}

void PlayerCamera::Render()
{
	Camera::Render();
}

bool PlayerCamera::BindComponent(CameraComponent* pComponent)
{
	if (pOwnerComponent_ != nullptr || pComponent != nullptr)
	{
		return false;
	}

	pOwnerComponent_ = pComponent;
	return true;
}

void PlayerCamera::CleanUp()
{
}
