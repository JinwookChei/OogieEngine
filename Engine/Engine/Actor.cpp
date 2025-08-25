#include "stdafx.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"


Actor::Actor()
	:levelLink_(),
	pRenderer_(nullptr)
{
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