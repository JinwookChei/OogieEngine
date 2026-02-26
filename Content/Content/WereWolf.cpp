#include "stdafx.h"
#include "WereWolf.h"

WereWolf::WereWolf()
{
	pMovementComponent_ = CreateComponent<MovementComponent>();
	pSkeletalMeshComponent_ = CreateComponent<SkeletalMeshComponent>();
}

WereWolf::~WereWolf()
{
	CleanUp();
}

void WereWolf::Tick(double deltaTime)
{
	Player::Tick(deltaTime);

	if (InputManager::IsPress('1'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(1);
	}
	if (InputManager::IsPress('2'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(2);
	}
	if (InputManager::IsPress('3'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(3);
	}
	if (InputManager::IsPress('4'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(4);
	}
	if (InputManager::IsPress('5'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(5);
	}
	if (InputManager::IsPress('6'))
	{
		pSkeletalMeshComponent_->ChangeAnimation(6);
	}

}

void WereWolf::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial1;
	IMaterial* pMaterial2;
	MeshManager::Instance()->GetMesh(&pMesh, 14);
	MaterialManager::Instance()->GetMaterial(&pMaterial1, 13);
	MaterialManager::Instance()->GetMaterial(&pMaterial2, 14);
	pSkeletalMeshComponent_->InitPSO(1, 2, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pSkeletalMeshComponent_->GetPSO()->SetMeshToSlot(0, pMesh);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(0, pMaterial1);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(1, pMaterial2);
	//pSkeletalMeshComponent_->ChangeAnimation(1);
	pSkeletalMeshComponent_->SetSkeleton(14);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void WereWolf::Render()
{
	Player::Render();
}

void WereWolf::CleanUp()
{
}
