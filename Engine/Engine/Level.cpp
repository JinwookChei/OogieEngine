#include "stdafx.h"
#include "Actor.h"	
#include "Level.h"

Level::Level()
	: actorHead_(nullptr),
	actorTail_(nullptr)
{
}

Level::~Level()
{
	CleanUp();
}


void Level::UpdateTick(unsigned long long deltaTime)
{
	LINK_ITEM* pItem = actorHead_;
	while (pItem)
	{
		Actor* pActor = (Actor*)pItem->item_;

		pActor->Tick(deltaTime);

		pItem = pItem->next_;
	}
	
	Tick(deltaTime);
}

void Level::SpawnActorInternal(Actor* pActor)
{
	if (nullptr == pActor)
	{
		return;
	}

	RegisterActor(pActor);

	pActor->BeginPlay();
}

void Level::RegisterActor(Actor* pActor)
{
	LinkToLinkedList(&actorHead_, &actorTail_, pActor->LevelLink());
}

void Level::CleanUp()
{
	while (actorHead_)
	{
		Actor* actor = (Actor*)actorHead_->item_;

		UnLinkFromLinkedList(&actorHead_, &actorTail_, actor->LevelLink());

		delete actor;
	}
}
