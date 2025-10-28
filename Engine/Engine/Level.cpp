#include "stdafx.h"
#include "Actor.h"	
#include "Level.h"

Level::Level()
	: pCameraHead_(nullptr),
	pCameraTail_(nullptr),
	pActorGroupHead_(nullptr),
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

void Level::SpawnCameraInternal(Camera* pCamera)
{
	if (nullptr == pCamera)
	{
		return;
	} 

	RegisterCamera(pCamera);

	pCamera->BeginPlay();
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
	
	LinkToLinkedListFIFO(&pActorGroupHead_, &pActorGroupTail_, &pNewActorGroup->groupLink_);

	LinkToLinkedListFIFO(&pNewActorGroup->pActorHead_, &pNewActorGroup->pActorTail_, pActor->LevelLink());
}

void Level::RegisterCamera(Camera* pCamera)
{
	if (nullptr == pCamera)
	{
		return;
	}
	
	LinkToLinkedListFIFO(&pCameraHead_, &pCameraTail_, pCamera->LevelLink());
}

void Level::OnTick(double deltaTime)
{
	OnTickCameras(deltaTime);
	OnTickActors(deltaTime);
	Tick(deltaTime);
}

void Level::OnTickCameras(double deltaTime)
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		Camera* pCurCamera = (Camera*)pCameraIter->item_;
		pCurCamera->Tick(deltaTime);
		pCameraIter = pCameraIter->next_;
	}
}

void Level::OnTickActors(double deltaTime)
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
}

void Level::OnRender()
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		GCurrentCamera = (Camera*)pCameraIter->item_;
		GCurrentCamera->CameraRenderBegin();
		
		OnRenderCameras();
		OnRenderActors();
		pCameraIter = pCameraIter->next_;
		GCurrentCamera->CameraRenderEnd();
	}
}

void Level::OnRenderCameras()
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		Camera* pCurCamera = (Camera*)pCameraIter->item_;
		if (pCurCamera != GCurrentCamera)
		{
			pCurCamera->Render();
		}
		pCameraIter = pCameraIter->next_;
	}
}

void Level::OnRenderActors()
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

void Level::BlitCameraToBackBuffer()
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		Camera* pCurCamera = (Camera*)pCameraIter->item_;
		pCurCamera->BlitToBackBuffer();
		pCameraIter = pCameraIter->next_;
	}
}


void Level::CleanUp()
{
	CleanUpCamera();

	CleanUpActorGroup();
}

void Level::CleanUpCamera()
{
	while (pCameraHead_)
	{
		Camera* pCurCamera = (Camera*)pCameraHead_->item_;
		UnLinkFromLinkedList(&pCameraHead_, &pCameraTail_, pCurCamera->LevelLink());
		delete pCurCamera;
	}
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
