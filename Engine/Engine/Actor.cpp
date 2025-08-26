#include "stdafx.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"


Actor::Actor()
	: pRenderer_(new Renderer)
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
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}

void Actor::Render()
{

}
