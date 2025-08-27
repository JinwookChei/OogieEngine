#include "stdafx.h"
#include "Actor.h"
#include "RenderComponent.h"
#include "Mesh.h"
#include "Material.h"


Actor::Actor()
	: pRenderComponent_(new RenderComponent) //TODO
{
	levelLink_.prev_ = nullptr;
	levelLink_.next_ = nullptr;
	levelLink_.item_ = this;
}

Actor::~Actor()
{
	CleanUp();
}

LINK_ITEM* Actor::LevelLink()
{
	return &levelLink_;
}

void Actor::CleanUp()
{
	if (nullptr != pRenderComponent_)
	{
		delete pRenderComponent_;
		pRenderComponent_ = nullptr;
	}
}

void Actor::Render()
{
	pRenderComponent_->Render();
}
