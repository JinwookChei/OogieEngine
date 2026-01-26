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
	//SamplerManager::Instance()->Setting(0, E_SAMPLER_TYPE::LINEAR_CLAMP);
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
		GCurrentCamera->LightPassBegin();
		GRenderer->RenderLightBegin(GCurrentCamera->GetGBufferTarget());
		OnRenderLights();
		GRenderer->RenderLightEnd(GCurrentCamera->GetGBufferTarget());
		GCurrentCamera->LightPassEnd();
		// Light Pass End


		// Particle Pass Begin
		GCurrentCamera->ParticlePassBegin();
		OnRenderParticles();
		GCurrentCamera->ParticlePassEnd();


		GCurrentCamera->pFinalRenderTarget_->Bind();
		GCurrentCamera->UpdatePerFrameConstant();
		GRenderer->RenderMerge(GCurrentCamera->pGBufferRenderTarget_, GCurrentCamera->pParticleRenderTarget_);
		GCurrentCamera->pFinalRenderTarget_->EndRenderPass();


		// DebugRender
		GCurrentCamera->pDebugRenderTarget_->Clear();
		GCurrentCamera->pDebugRenderTarget_->Bind();
		GRenderer->RenderDebug();
		GCurrentCamera->pDebugRenderTarget_->EndRenderPass();
		// DebugRender End
		

		GCurrentCamera->pFinalRenderTarget_->Bind();
		GCurrentCamera->UpdatePerFrameConstant();
		GRenderer->RenderMerge(GCurrentCamera->pGBufferRenderTarget_, GCurrentCamera->pDebugRenderTarget_);
		GCurrentCamera->pFinalRenderTarget_->EndRenderPass();
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

void Level::OnRenderLights()
{
	LINK_NODE* pLightIter = actorList_[(int)E_ACTOR_TYPE::LIGHT].GetHead();
	while (pLightIter)
	{
		Light* pCurLight = static_cast<Light*>(pLightIter->pItem_);
		pLightIter = pLightIter->next_;
		GRenderer->RenderLight(pCurLight->GetData());
		//pCurLight->BindLight();
		//GCurrentCamera->RenderLight();
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
	GRenderer->RenderParticles(GParticle_1, world1, viewProj, cameraRight, cameraUp);

	Float4 scale2 = { 1.0f, 1.0f, 1.0f, 0.0f };
	Float4 rotation2 = { 0.0f, 0.0f, 0.0f , 0.0f };
	Float4 position2 = { 0.0f, 5.0f, 5.0f , 1.0f };
	Float4x4 world2;
	MATH::MatrixCompose(world2, scale2, rotation2, position2);
	GRenderer->RenderParticles(GParticle_2, world2,  viewProj, cameraRight, cameraUp);
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