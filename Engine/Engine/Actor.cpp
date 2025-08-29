#include "stdafx.h"
#include "Actor.h"
#include "RenderComponent.h"
#include "Mesh.h"
#include "Material.h"


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

	//if (nullptr != pRenderComponent_)
	//{
	//	delete pRenderComponent_;
	//	pRenderComponent_ = nullptr;
	//}
}

//void Actor::Render()
//{
//	//pRenderComponent_->Render();
//}
