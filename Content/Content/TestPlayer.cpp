#include "stdafx.h"
#include "TestPlayer.h"

TestPlayer::TestPlayer()
	: pSkeletalMeshComponent_(new SkeletalMeshComponent(this))
{
}

TestPlayer::~TestPlayer()
{
	CleanUp();
}

void TestPlayer::Tick(double deltaTime)
{
	pSkeletalMeshComponent_->Tick(deltaTime);
}

void TestPlayer::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 13);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 11);
	pSkeletalMeshComponent_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pSkeletalMeshComponent_->GetPSO()->SetMeshToSlot(0, pMesh);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(0, pMaterial);
	pSkeletalMeshComponent_->ChangeAnimation(0);
	pSkeletalMeshComponent_->SetSkeleton(13);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void TestPlayer::Render()
{
	//pRenderer_->Render();
	pSkeletalMeshComponent_->Render();
}

void TestPlayer::CleanUp()
{
	if (nullptr != pSkeletalMeshComponent_)
	{
		delete pSkeletalMeshComponent_;
		pSkeletalMeshComponent_ = nullptr;
	}
}
