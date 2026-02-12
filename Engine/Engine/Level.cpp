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



		// 기존. 라이트 랜더링
		//GCurrentCamera->RenderPassBegin(E_RENDER_PASS_TYPE::LightPass);
		//Renderer::Instance()->RenderLightBegin(GCurrentCamera->GetGBufferTarget());
		//OnRenderLights();
		//Renderer::Instance()->RenderLightEnd(GCurrentCamera->GetGBufferTarget());
		//GCurrentCamera->RenderPassEnd();
		//
		// Light Pass
		// Renderer::Instance()->RenderLightBegin(GCurrentCamera->GetGBufferTarget());
		// OnRenderLights();



		// Light Pass End


		//// Particle Pass Begin
		//GCurrentCamera->ParticlePassBegin();
		//OnRenderParticles();
		//GCurrentCamera->ParticlePassEnd();
		//// Particle Pass End

		//GCurrentCamera->pFinalRenderTarget_->Bind();
		//GCurrentCamera->UpdatePerFrameConstant();
		//Renderer::Instance()->RenderMerge(GCurrentCamera->pParticleRenderTarget_);
		////GRenderer->RenderMerge(GCurrentCamera->pGBufferRenderTarget_, GCurrentCamera->pParticleRenderTarget_);
		//GCurrentCamera->pFinalRenderTarget_->EndRenderPass();


		//// DebugRender
		//GCurrentCamera->DebugPassBegin();
		////GCurrentCamera->pDebugRenderTarget_->Clear();
		////GCurrentCamera->pDebugRenderTarget_->Bind();
		//Renderer::Instance()->RenderDebug();
		//GCurrentCamera->DebugPassEnd();
		//// DebugRender End
		

		//GCurrentCamera->pFinalRenderTarget_->Bind();
		//GCurrentCamera->UpdatePerFrameConstant();
		//Renderer::Instance()->RenderMerge(GCurrentCamera->pDebugRenderTarget_);
		////GRenderer->RenderMerge(GCurrentCamera->pGBufferRenderTarget_, GCurrentCamera->pDebugRenderTarget_);
		//GCurrentCamera->pFinalRenderTarget_->UnBind();
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
		if (i == (int)E_ACTOR_TYPE::LIGHT) continue;

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
	IMaterial* pLightMaterial = Light::GLightPSO->GetMaterial();
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
		pCurLight->Render();
		pLightIter = pLightIter->next_;
	}
}

void Level::OnRenderParticles()
{
	const Transform& worldForm = GCurrentCamera->GetWorldTransform();
	Vector eye = worldForm.GetPosition();
	Vector to = worldForm.ForwardVector();
	Vector right = worldForm.RightVector();
	Vector up = worldForm.UpVector();

	const Float2& size = GCurrentCamera->GetRenderSize();
	Float4x4 proj;
	MATH::MatrixPerspectiveFovLH(proj, GCurrentCamera->GetFov(), (size.X / size.Y), GCurrentCamera->GetNear(), GCurrentCamera->GetFar());
	Float4x4 view;
	MATH::MatrixLookToLH(view, eye, to, up);
	Float4x4 viewProj;
	MATH::MatrixMultiply(viewProj, view, proj);
	Float3 cameraRight(right.X, right.Y, right.Z);
	Float3 cameraUp(up.X, up.Y, up.Z);

	
	Float4 scale1 = { 1.0f, 1.0f, 1.0f, 0.0f };
	Float4 rotation1 = { 0.0f, 0.0f, 0.0f , 0.0f };
	Float4 position1 = { 0.0f, 0.0f, 0.0f , 1.0f };
	Float4x4 world1;
	MATH::MatrixCompose(world1, scale1, rotation1, position1);
	Renderer::Instance()->RenderParticles(GParticle_1, world1, viewProj, cameraRight, cameraUp);

	Float4 scale2 = { 1.0f, 1.0f, 1.0f, 0.0f };
	Float4 rotation2 = { 0.0f, 0.0f, 0.0f , 0.0f };
	Float4 position2 = { 0.0f, 5.0f, 5.0f , 1.0f };
	Float4x4 world2;
	MATH::MatrixCompose(world2, scale2, rotation2, position2);
	Renderer::Instance()->RenderParticles(GParticle_2, world2,  viewProj, cameraRight, cameraUp);
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
}