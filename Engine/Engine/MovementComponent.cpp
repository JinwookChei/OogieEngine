#include "stdafx.h"
#include "RunTimeMode.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent()
	: velocity_(1.0f)
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::BeginPlay()
{
}

void MovementComponent::Tick(double deltaTime)
{
	ActorComponent::Tick(deltaTime);

	if (RunTimeMode::GetCurrentMode() != E_RUNTIME_MODE::GAME) return;
	if (InputManager::IsPress('W'))
	{
		Float4 forwardVector = GetOwner()->GetWorldTransform().ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, velocity_ * (float)deltaTime);
		GetOwner()->GetWorldTransform().AddPosition(offset);
	}
	if (InputManager::IsPress('S'))
	{
		Float4 forwardVector = GetOwner()->GetWorldTransform().ForwardVector();
		Float4 offset;
		MATH::VectorScale(offset, forwardVector, -1.0f * velocity_ * (float)deltaTime);
		GetOwner()->GetWorldTransform().AddPosition(offset);
	}
	if (InputManager::IsPress('A'))
	{
		Float4 rightVector = GetOwner()->GetWorldTransform().RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, -1.0f * velocity_ * (float)deltaTime);
		GetOwner()->GetWorldTransform().AddPosition(offset);
	}
	if (InputManager::IsPress('D'))
	{
		Float4 rightVector = GetOwner()->GetWorldTransform().RightVector();
		Float4 offset;
		MATH::VectorScale(offset, rightVector, velocity_ * (float)deltaTime);
		GetOwner()->GetWorldTransform().AddPosition(offset);
	}
}

void MovementComponent::Render()
{
	ActorComponent::Render();
}

void MovementComponent::CleanUp()
{

}
