#include "stdafx.h"
#include "RenderComponent.h"
#include "BoundVolume.h"
#include "Actor.h"



Actor::Actor()
	: pRenderer_(new RenderComponent(this)),
	pTransform_(new Transform),
	pBoundVolume(new BoundVolume)
{
	levelLink_.prev_ = nullptr;
	levelLink_.next_ = nullptr;
	levelLink_.pItem_ = this;

	pBoundVolume->Init(pRenderer_);
}

Actor::~Actor()
{
	CleanUp();
}

void Actor::Render()
{

}

Transform& Actor::GetWorldTransform() const
{
	return *pTransform_;
}

LINK_NODE* Actor::LevelLink()
{
	return &levelLink_;
}

void Actor::CleanUp()
{
	if (nullptr != pBoundVolume)
	{
		delete pBoundVolume;
		pBoundVolume = nullptr;
	}
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
