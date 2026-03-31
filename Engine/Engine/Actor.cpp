#include "stdafx.h"
#include "ActorComponent.h"
#include "BoundVolume.h"
#include "Actor.h"


Actor::Actor()
	: actorType_(E_ACTOR_TYPE::NONE)
	, pTransform_(new Transform)
	, ownedComponents_()
{
	levelLink_.prev_ = nullptr;
	levelLink_.next_ = nullptr;
	levelLink_.pItem_ = this;

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

Actor::~Actor()
{
	CleanUp();
}

void Actor::Tick(double deltaTime)
{
	for (auto& pair : ownedComponents_)
	{
		ActorComponent* component = pair.second;
		if (component && component->IsEnable())
		{
			component->Tick(deltaTime);
		}
	}
}

void Actor::Render()
{
	for (auto& pair : ownedComponents_)
	{
		ActorComponent* component = pair.second;
		if (component && component->IsEnable())
		{
			component->Render();
		}
	}
}

void Actor::ParticleRender()
{

}

Transform& Actor::GetWorldTransform() const
{
	return *pTransform_;
}

void Actor::DeleteComponents()
{
	for (auto& pair : ownedComponents_)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	ownedComponents_.clear();
}

LINK_NODE* Actor::LevelLink()
{
	return &levelLink_;
}

E_ACTOR_TYPE Actor::GetActorType() const
{
	return actorType_;
}

void Actor::CleanUp()
{
	DeleteComponents();

	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
}

ENGINE_API IEditorBindTransform& __stdcall Actor::GetTransformForEditor()
{
	return *pTransform_;
}