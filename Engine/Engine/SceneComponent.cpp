#include "stdafx.h"
#include "ActorComponent.h"
#include "SceneComponent.h"

SceneComponent::SceneComponent()
	: pTransform_(nullptr)
{
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

void SceneComponent::CleanUp()
{
}