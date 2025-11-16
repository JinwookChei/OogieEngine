#include "stdafx.h"
#include "Actor.h"	
#include "Level.h"

Level::Level()
	: pCameraHead_(nullptr),
	pCameraTail_(nullptr),
	pLightHead_(nullptr),
	pLightTail_(nullptr),
	pActorGroupHead_(nullptr),
	pActorGroupTail_(nullptr)
{
}

Level::~Level()
{
	CleanUp();
}

void Level::SpawnActorInternal(Actor* pActor, E_ACTOR_TYPE actorType)
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

void Level::SpawnLightInternal(Light* pLight)
{
	if (nullptr == pLight)
	{
		return;
	}

	RegisterLight(pLight);

	pLight->BeginPlay();
}


void Level::RegisterActor(Actor* pActor, E_ACTOR_TYPE actorType)
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

void Level::RegisterLight(Light* pLight)
{
	if (nullptr == pLight)
	{
		return;
	}

	LinkToLinkedListFIFO(&pLightHead_, &pLightTail_, pLight->LevelLink());
}

void Level::OnTick(double deltaTime)
{
	OnTickLights(deltaTime);
	OnTickActors(deltaTime);
	OnTickCameras(deltaTime);
	Tick(deltaTime);
}

void Level::OnTickCameras(double deltaTime)
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		Camera* pCurCamera = static_cast<Camera*>(pCameraIter->item_);
		pCurCamera->Tick(deltaTime);
		pCameraIter = pCameraIter->next_;
	}
}

void Level::OnTickLights(double deltaTime)
{
	LINK_ITEM* pLightIter = pLightHead_;
	while (pLightIter)
	{
		Light* pCurLight = static_cast<Light*>(pLightIter->item_);
		pCurLight->Tick(deltaTime);
		pLightIter = pLightIter->next_;
	}
}

void Level::OnTickActors(double deltaTime)
{
	LINK_ITEM* pGroupIter = pActorGroupHead_;
	while (pGroupIter)
	{
		ActorGroupContainer* pActorGroup = static_cast<ActorGroupContainer*>(pGroupIter->item_);

		LINK_ITEM* pActorIter = pActorGroup->pActorHead_;
		while (pActorIter)
		{
			Actor* pActor = static_cast<Actor*>(pActorIter->item_);
			pActor->Tick(deltaTime);
			pActorIter = pActorIter->next_;
		}
		pGroupIter = pGroupIter->next_;
	}
}

void Level::OnRender()
{
	SamplerManager::Instance()->Setting(E_SAMPLER_MODE::LINEAR_CLAMP);

	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		GCurrentCamera = static_cast<Camera*>(pCameraIter->item_);
		pCameraIter = pCameraIter->next_;

		// Update FrameConstant
		GCurrentCamera->UpdatePerFrameConstant();

		// Geometry Pass
		GCurrentCamera->GeometryPassBegin();

		OnRenderCameras();
		OnRenderActors();
		OnRenderLights();

		GCurrentCamera->GeometryPassEnd();
		// Geometry Pass End

		
		// Light Pass
		GBlendState->Setting();
		GCurrentCamera->LightPassBegin();
		LINK_ITEM* pLightIter = pLightHead_;
		while (pLightIter)
		{
			Light* curLight = static_cast<Light*>(pLightIter->item_);
			pLightIter = pLightIter->next_;
			curLight->BindLight();
			GCurrentCamera->RenderLight();
		}


		//GDebugRenderer->SetViewProj(GCurrentCamera->View(), GCurrentCamera->Projection());
		//GDebugRenderer->RenderAll();
		//GCurrentCamera->UpdatePerFrameConstant();

		GCurrentCamera->LightPassEnd();
		GBlendState->Clear();
		// Light Pass End
	}
}

void Level::OnRenderCameras()
{
	LINK_ITEM* pCameraIter = pCameraHead_;
	while (pCameraIter)
	{
		Camera* pCurCamera = static_cast<Camera*>(pCameraIter->item_);
		if (pCurCamera != GCurrentCamera)
		{
			pCurCamera->Render();
		}
		pCameraIter = pCameraIter->next_;
	}
}

void Level::OnRenderLights()
{
	LINK_ITEM* pLightIter = pLightHead_;
	while (pLightIter)
	{
		Light* pCurLight = static_cast<Light*>(pLightIter->item_);
		pCurLight->Render();
		pLightIter = pLightIter->next_;
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
			Actor* pActor = static_cast<Actor*>(pActorIter->item_);
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
		Camera* pCurCamera = static_cast<Camera*>(pCameraIter->item_);
		pCurCamera->BlitToBackBuffer();
		pCameraIter = pCameraIter->next_;
	}
}


void Level::CleanUp()
{
	CleanUpCamera();

	CleanUpLight();

	CleanUpActorGroup();
}

void Level::CleanUpCamera()
{
	while (pCameraHead_)
	{
		Camera* pCurCamera = static_cast<Camera*>(pCameraHead_->item_);
		UnLinkFromLinkedList(&pCameraHead_, &pCameraTail_, pCurCamera->LevelLink());
		delete pCurCamera;
	}
}

void Level::CleanUpLight()
{
	while (pLightHead_)
	{
		Light* pCurLight = static_cast<Light*>(pLightHead_->item_);
		UnLinkFromLinkedList(&pLightHead_, &pLightTail_, pCurLight->LevelLink());
		delete pCurLight;
	}
}

void Level::CleanUpActorGroup()
{
	while (pActorGroupHead_)
	{
		ActorGroupContainer* pActorGroup = static_cast<ActorGroupContainer*>(pActorGroupHead_->item_);
		while (pActorGroup->pActorHead_)
		{
			Actor* pActor = static_cast<Actor*>(pActorGroup->pActorHead_->item_);
			UnLinkFromLinkedList(&pActorGroup->pActorHead_, &pActorGroup->pActorTail_, pActor->LevelLink());
			delete pActor;
		}

		UnLinkFromLinkedList(&pActorGroupHead_, &pActorGroupTail_, &pActorGroup->groupLink_);
		delete pActorGroup;
	}
}
