#include "stdafx.h"
#include "Actor.h"
#include "RenderComponent.h"


Actor::Actor()
	:pTransform_(new Transform)
	//pRenderComponent_(new RenderComponent(this)) //TODO
{
	levelLink_.prev_ = nullptr;
	levelLink_.next_ = nullptr;
	levelLink_.item_ = this;
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

LINK_ITEM* Actor::LevelLink()
{
	return &levelLink_;
}

void Actor::CleanUp()
{
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
}
