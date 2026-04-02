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

ENGINE_API E_ACTOR_TYPE __stdcall Actor::GetActorTypeForEditor() const
{
	return GetActorType();
}

int __stdcall Actor::GetLightTypeForEditor() const
{
	return -1;
}

Color __stdcall Actor::GetDiffuseColorForEditor() const
{
	return Color(0.0f, 0.0f, 0.0f, 0.0f);
}

Color __stdcall Actor::GetSpecularColorForEditor() const
{
	return Color(0.0f, 0.0f, 0.0f, 0.0f);
}

Color __stdcall Actor::GetAmbientColorForEditor() const
{
	return Color(0.0f, 0.0f, 0.0f, 0.0f);
}

void __stdcall Actor::SetDiffuseColorForEditor(const Color& color)
{
	return;
}

void __stdcall Actor::SetSpecularColorForEditor(const Color& color)
{
	return;
}

void __stdcall Actor::SetAmbientColorForEditor(const Color& color)
{
	return;
}

float __stdcall Actor::GetLightIntensity() const
{
	return 0.0f;
}

float __stdcall Actor::GetLightRange() const
{
	return 0.0f;
}

float __stdcall Actor::GetLightSmooth() const
{
	return 0.0f;
}

float __stdcall Actor::GetLightInnerAngle() const
{
	return 0.0f;
}

float __stdcall Actor::GetLightOuterAngle() const
{
	return 0.0f;
}

void __stdcall Actor::SetLightIntensity(float intensity)
{
	return;
}

void __stdcall Actor::SetLightRange(float range)
{
	return;
}

void __stdcall Actor::SetLightSmooth(float smooth)
{
	return;
}

void __stdcall Actor::SetLightInnerAngle(float innerAngle)
{
	return;
}

void __stdcall Actor::SetLightOuterAngle(float outerAngle)
{
	return;
}
