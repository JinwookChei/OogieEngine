#include "stdafx.h"
#include "Actor.h"	
#include "Level.h"

Level::Level()
	//: pActorList_(new LinkedList[(int)E_ACTOR_TYPE::MAX])
	: actorList_()
{
}

Level::~Level()
{
	CleanUp();
}

//LinkedList* Level::GetActorList() const
//{
//	return pActorList_;
//}

LinkedList* Level::GetActorList(const E_ACTOR_TYPE& actoryType)
{
	return &actorList_[(int)actoryType];
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

	RegisterActor(pCamera, E_ACTOR_TYPE::CAMERA);

	pCamera->BeginPlay();
}

void Level::SpawnLightInternal(Light* pLight)
{
	if (nullptr == pLight)
	{
		return;
	}

	RegisterActor(pLight, E_ACTOR_TYPE::LIGHT);

	pLight->BeginPlay();
}


//void Level::RegisterActor(Actor* pActor, E_ACTOR_TYPE actorType)
//{
//	if (nullptr == pActor)
//	{
//		return;
//	}
//
//	pActorList_[(int)actorType].PushBack(pActor->LevelLink());
//}

void Level::RegisterActor(Actor* pActor, E_ACTOR_TYPE actorType)
{
	if (nullptr == pActor)
	{
		return;
	}

	actorList_[(int)actorType].PushBack(pActor->LevelLink());
}


void Level::OnTick(double deltaTime)
{
	OnActorTick(deltaTime);

	Tick(deltaTime);
}

void Level::OnRender()
{
	SamplerManager::Instance()->Setting(0, E_SAMPLER_TYPE::LINEAR_CLAMP);
	RasterizerManager::Instance()->Setting(E_FILLMODE_TYPE::SOLID);

	LINK_NODE* pCameraIter = actorList_[(int)E_ACTOR_TYPE::CAMERA].GetHead();
	while (pCameraIter)
	{
		GCurrentCamera = static_cast<Camera*>(pCameraIter->pItem_);
		pCameraIter = pCameraIter->next_;

		// Update FrameConstant
		GCurrentCamera->UpdatePerFrameConstant();

		// Geometry Pass
		GCurrentCamera->GeometryPassBegin();
		OnRenderActors();
		GCurrentCamera->GeometryPassEnd();
		// Geometry Pass End

		// Light Pass
		BlendStateManager::Instance()->Setting(E_BLEND_MODE_TYPE::ADDITIVE_BLEND);
		GCurrentCamera->LightPassBegin();
		OnLightPass();
		GCurrentCamera->LightPassEnd();
		BlendStateManager::Instance()->Clear();
		// Light Pass End

		////////////////////////////////////////////////////////////////////////////////// 디버깅 임시용 //////////////////////////////////////////////////////////////////////.
		// DebugRender
		
		GCurrentCamera->pDebugBufferTarget_->Clear();
		GCurrentCamera->pDebugBufferTarget_->Setting();
		GDebugRenderer->SetViewProj(GCurrentCamera->View(), GCurrentCamera->Projection());
		GDebugRenderer->RenderAll();
		GCurrentCamera->pDebugBufferTarget_->EndRenderPass();
		// DebugRender End
		
		BlendStateManager::Instance()->Setting(E_BLEND_MODE_TYPE::OPAQUE_BLEND);
		GCurrentCamera->pGBufferTarget_->BindRenderTextureForPS(0);
		GCurrentCamera->pDebugBufferTarget_->BindRenderTextureForPS(4);

		// Begin
		GCurrentCamera->UpdatePerFrameConstant();
		GCurrentCamera->pLightBufferTarget_->Setting();
		GCurrentCamera->pScreenVertex_->Setting();
		GCurrentCamera->pScreenInputLayout_->Setting();
		GCurrentCamera->pDebugBufferMaterial_->Setting();
	
		// Draw
		GCurrentCamera->pScreenVertex_->Draw();

		// End
		GCurrentCamera->pLightBufferTarget_->EndRenderPass();
		
		GCurrentCamera->pDebugBufferTarget_->ClearRenderTextureForPS(4);
		GCurrentCamera->pGBufferTarget_->ClearRenderTextureForPS(0);
		BlendStateManager::Instance()->Clear();
		////////////////////////////////////////////////////////////////////////////////// 디버깅 임시용 //////////////////////////////////////////////////////////////////////..
	}
}

void Level::OnActorTick(double deltaTime)
{
	for (int i = 0; i < (int)E_ACTOR_TYPE::MAX; ++i)
	{
		LINK_NODE* pCurLink = actorList_[i].GetHead();
		while (pCurLink)
		{
			Actor* pCurActor = static_cast<Actor*>(pCurLink->pItem_);
			pCurActor->Tick(deltaTime);

			pCurLink = pCurLink->next_;
		}
	}
}

void Level::OnRenderActors()
{
	for (int i = 0; i < (int)E_ACTOR_TYPE::MAX; ++i)
	{
		LINK_NODE* pActorIter = actorList_[i].GetHead();
		while (pActorIter)
		{
			Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);
			pActor->Render();
			pActorIter = pActorIter->next_;
		}
	}
}

void Level::OnLightPass()
{
	LINK_NODE* pLightIter = actorList_[(int)E_ACTOR_TYPE::LIGHT].GetHead();
	while (pLightIter)
	{
		Light* pCurLight = static_cast<Light*>(pLightIter->pItem_);
		pLightIter = pLightIter->next_;
		pCurLight->BindLight();
		GCurrentCamera->RenderLight();
	}
}

void Level::BlitCameraToBackBuffer()
{
	LINK_NODE* pCameraIter = actorList_[(int)E_ACTOR_TYPE::CAMERA].GetHead();
	while (pCameraIter)
	{
		Camera* pCurCamera = static_cast<Camera*>(pCameraIter->pItem_);
		pCurCamera->BlitToBackBuffer();
		pCameraIter = pCameraIter->next_;
	}
}


void Level::CleanUp()
{
	CleanUpActors();
}

void Level::CleanUpActors()
{
	for (int i = 0; i < (int)E_ACTOR_TYPE::MAX; ++i)
	{
		LINK_NODE* pActorIter = actorList_[i].GetHead();
		while (pActorIter)
		{
			actorList_[i].Remove(pActorIter);
			Actor* pCurActor = static_cast<Actor*>(pActorIter->pItem_);
			if (nullptr != pCurActor)
			{
				delete pCurActor;
				pCurActor = nullptr;
			}

			pActorIter = actorList_[i].GetHead();
		}
	}

	/*if (nullptr != pActorList_)
	{
		delete[] pActorList_;
		pActorList_ = nullptr;
	}*/
}