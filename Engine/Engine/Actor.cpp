#include "stdafx.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Material.h"


Actor::Actor()
	:pRenderer_(nullptr)
{
}

Actor::~Actor()
{
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
