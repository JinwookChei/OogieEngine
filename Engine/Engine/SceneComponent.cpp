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

void SceneComponent::CleanUp()
{
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
}