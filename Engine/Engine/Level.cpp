#include "stdafx.h"
#include "Actor.h"	
#include "Level.h"

Level::Level()
	: pActorGroupHead_(nullptr),
	pActorGroupTail_(nullptr)
{
}

Level::~Level()
{
	CleanUp();
}

void Level::SpawnActorInternal(Actor* pActor, ACTOR_TYPE actorType)
{
	if (nullptr == pActor)
	{
		return;
	}

	RegisterActor(pActor, actorType);

	pActor->BeginPlay();
}


void Level::RegisterActor(Actor* pActor, ACTOR_TYPE actorType)
{
	if (nullptr == pActor)
	{
		return;
	}

	LINK_ITEM* pGroupIter = pActorGroupHead_;
	while (pGroupIter)
	{
		ActorGroupContainer* pActorGroup = (ActorGroupContainer*)pGroupIter->item_;
		if (actorType == pActorGroup->actorType_)
		{
			LinkToLinkedList(&pActorGroup->pActorHead_, &pActorGroup->pActorTail_, pActor->LevelLink());
			return;
		}

		pGroupIter = pGroupIter->next_;
	}

	ActorGroupContainer* pNewActorGroup = new ActorGroupContainer;
	pNewActorGroup->pActorHead_ = nullptr;
	pNewActorGroup->pActorTail_ = nullptr;
	pNewActorGroup->groupLink_.next_ = nullptr;
	pNewActorGroup->groupLink_.prev_ = nullptr;
	pNewActorGroup->groupLink_.item_ = pNewActorGroup;
	pNewActorGroup->actorType_ = actorType;
	
	LinkToLinkedList(&pActorGroupHead_, &pActorGroupTail_, &pNewActorGroup->groupLink_);

	LinkToLinkedList(&pNewActorGroup->pActorHead_, &pNewActorGroup->pActorTail_, pActor->LevelLink());
}

void Level::OnTick(double deltaTime)
{
	LINK_ITEM* pGroupIter = pActorGroupHead_;
	while (pGroupIter)
	{
		ActorGroupContainer* pActorGroup = (ActorGroupContainer*)pGroupIter->item_;

		LINK_ITEM* pActorIter = pActorGroup->pActorHead_;
		while (pActorIter)
		{
			Actor* pActor = (Actor*)pActorIter->item_;
			pActor->Tick(deltaTime);
			pActorIter = pActorIter->next_;
		}
		pGroupIter = pGroupIter->next_;
	}
	
	Tick(deltaTime);
}

void Level::OnRender()
{
	LINK_ITEM* pGroupIter = pActorGroupHead_;
	while (pGroupIter)
	{
		ActorGroupContainer* pActorGroup = (ActorGroupContainer*)pGroupIter->item_;

		LINK_ITEM* pActorIter = pActorGroup->pActorHead_;
		while (pActorIter)
		{
			Actor* pActor = (Actor*)pActorIter->item_;
			pActor->Render();
			pActorIter = pActorIter->next_;
		}
		pGroupIter = pGroupIter->next_;
	}
}

void Level::CleanUp()
{
	CleanUpActorGroup();
}

void Level::CleanUpActorGroup()
{
	while (pActorGroupHead_)
	{
		ActorGroupContainer* pActorGroup = (ActorGroupContainer*)pActorGroupHead_->item_;
		while (pActorGroup->pActorHead_)
		{
			Actor* pActor = (Actor*)pActorGroup->pActorHead_->item_;
			UnLinkFromLinkedList(&pActorGroup->pActorHead_, &pActorGroup->pActorTail_, pActor->LevelLink());
			delete pActor;
		}

		UnLinkFromLinkedList(&pActorGroupHead_, &pActorGroupTail_, &pActorGroup->groupLink_);
		delete pActorGroup;
	}
}
