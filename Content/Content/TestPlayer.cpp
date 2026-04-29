#include "stdafx.h"
#include "TestPlayer.h"

TestPlayer::TestPlayer()
{
	pSkeletalMeshComponent_ = CreateComponent<SkeletalMeshComponent>();
}

TestPlayer::~TestPlayer()
{
	CleanUp();
}

void TestPlayer::Tick(double deltaTime)
{
	Player::Tick(deltaTime);

	if (InputManager::IsPress('9'))
	{
		pSkeletalMeshComponent_->ChangeAnimation("AM_Capoeira", 11);
	}
}

void TestPlayer::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, "MS_Capoeira", 11);
	MaterialManager::Instance()->GetMaterial(&pMaterial, "MT_Maria", 8);
	pSkeletalMeshComponent_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pSkeletalMeshComponent_->GetPSO()->SetMeshToSlot(0, pMesh);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(0, pMaterial);
	pSkeletalMeshComponent_->SetSkeleton("SK_Campeira", 11);
	pSkeletalMeshComponent_->ChangeAnimation("AM_Capoeira", 11);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void TestPlayer::Render()
{
	Player::Render();
}

void TestPlayer::CleanUp()
{
}
