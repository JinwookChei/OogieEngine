#include "stdafx.h"
#include "ActorComponent.h"

ActorComponent::ActorComponent()
	: pOwner_(nullptr)
	, isEnable_(true)
{
}

ActorComponent::~ActorComponent()
{
	CleanUp();
}

void ActorComponent::BeginPlay()
{
}

void ActorComponent::Tick(double deltaTime)
{
}

void ActorComponent::Render()
{
}

Actor* ActorComponent::GetOwner() const
{
	return pOwner_;
}

void ActorComponent::SetOwner(Actor* pOwner)
{
	pOwner_ = pOwner;
}

void ActorComponent::SetEnable(bool bEnable)
{
	isEnable_ = bEnable;
}

bool ActorComponent::IsEnable() const
{
	return isEnable_;
}

void ActorComponent::CleanUp()
{
}