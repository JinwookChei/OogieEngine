#include "stdafx.h"
#include "Actor.h"
#include "Light.h"
#include "Level.h"

Level::Level()
	: actorList_()
{
}

Level::~Level()
{
	CleanUp();
}


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

	pActor->actorType_ = actorType;
	RegisterActor(pActor);
	pActor->BeginPlay();
}

void Level::RegisterActor(Actor* pActor)
{
	if (nullptr == pActor)
	{
		return;
	}
	actorList_[(int)pActor->GetActorType()].PushBack(pActor->LevelLink());
}

void Level::OnTick(double deltaTime)
{
	OnActorTick(deltaTime);
	Tick(deltaTime);
}

void Level::OnRender()
{
	LINK_NODE* pCameraIter = actorList_[(int)E_ACTOR_TYPE::CAMERA].GetHead();
	while (pCameraIter)
	{
		GCurrentCamera = static_cast<Camera*>(pCameraIter->pItem_);
		pCameraIter = pCameraIter->next_;

		// Update FrameConstant
		GCurrentCamera->UpdatePerFrameConstant();

		// Geometry Pass
		GCurrentCamera->RenderPassBegin(E_RENDER_PASS_TYPE::GeometryPass);
		OnRenderActors();
		Renderer::Instance()->UnBindSRVs(true, true);
		GCurrentCamera->RenderPassEnd();
		// Geometry Pass End
		
		// Light Pass
		GCurrentCamera->RenderPassBegin(E_RENDER_PASS_TYPE::LightPass);
		OnRenderLights(GCurrentCamera->GetGBufferTarget());
		Renderer::Instance()->UnBindSRVs(true, true);
		GCurrentCamera->RenderPassEnd();
		// Light Pass End

		// Particle Pass
		GCurrentCamera->RenderPassBegin(E_RENDER_PASS_TYPE::ParticlePass);
		OnRenderParticles();
		GCurrentCamera->RenderPassEnd();
		// Particle Pass End

		// Debug Pass
		GCurrentCamera->RenderPassBegin(E_RENDER_PASS_TYPE::DebugPass);
		Debugger::Draw();
		GCurrentCamera->RenderPassEnd();
		// Debug Pass End
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

void Level::OnRenderLights(IRenderTarget* pGBufferTarget)
{
	IMaterial* pLightMaterial = Light::GLightPSO->GetMaterial(0);
	if (nullptr == pLightMaterial)
	{
		DEBUG_BREAK();
	}
	pLightMaterial->SetTextures(0, pGBufferTarget->GetRenderTexture(E_RENDER_TEXTURE_TYPE::Albedo));
	pLightMaterial->SetTextures(1, pGBufferTarget->GetRenderTexture(E_RENDER_TEXTURE_TYPE::Normal));
	pLightMaterial->SetTextures(2, pGBufferTarget->GetRenderTexture(E_RENDER_TEXTURE_TYPE::Specular));
	pLightMaterial->SetTextures(3, pGBufferTarget->GetRenderTexture(E_RENDER_TEXTURE_TYPE::Depth));

	LINK_NODE* pLightIter = actorList_[(int)E_ACTOR_TYPE::LIGHT].GetHead();
	while (pLightIter)
	{
		Light* pCurLight = static_cast<Light*>(pLightIter->pItem_);
		pCurLight->RenderLight();
		pLightIter = pLightIter->next_;
	}
}

void Level::OnRenderParticles()
{
	for (int i = 0; i < (int)E_ACTOR_TYPE::MAX; ++i)
	{
		if (i == (int)E_ACTOR_TYPE::LIGHT) continue;

		LINK_NODE* pActorIter = actorList_[i].GetHead();
		while (pActorIter)
		{
			Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);
			pActor->ParticleRender();
			pActorIter = pActorIter->next_;
		}
	}
}


void Level::BlitCameraToBackBuffer()
{
	LINK_NODE* pCameraIter = actorList_[(int)E_ACTOR_TYPE::CAMERA].GetHead();
	while (pCameraIter)
	{
		Camera* pCurCamera = static_cast<Camera*>(pCameraIter->pItem_);

		ITexture* pFianlRenderTex = pCurCamera->pFinalRenderTarget_->GetRenderTexture(E_RENDER_TEXTURE_TYPE::Albedo);
		GBlitPSO->GetMaterial(0)->SetTextures(0, pFianlRenderTex);
		Renderer::Instance()->Render(GBlitPSO);

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
}