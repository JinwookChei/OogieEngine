#include "stdafx.h"
#include "WereWolf.h"

WereWolf::WereWolf()
{
	pMovementComponent_ = CreateComponent<MovementComponent>();
	pSkeletalMeshComponent_ = CreateComponent<SkeletalMeshComponent>();
	pCameraComponent_ = CreateComponent<CameraComponent>();
}

WereWolf::~WereWolf()
{
	CleanUp();
}

void WereWolf::Tick(double deltaTime)
{
	Player::Tick(deltaTime);

	if (E_RUNTIME_MODE::GAME == RunTimeMode::GetCurrentMode())
	{
		if (InputManager::IsPress('W') && pSkeletalMeshComponent_->GetCurrentAnimationKey() != "AM_Werewolf_Walk")
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Walk", 16);
		}
		if (InputManager::IsUp('W'))
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Idle1", 17);
		}
		if (InputManager::IsPress('S') && pSkeletalMeshComponent_->GetCurrentAnimationKey() != "AM_Werewolf_Walk")
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Walk", 16);
		}
		if (InputManager::IsUp('S'))
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Idle1", 17);
		}
		if (InputManager::IsPress('A') && pSkeletalMeshComponent_->GetCurrentAnimationKey() != "AM_Werewolf_Walk")
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Walk", 16);
		}
		if (InputManager::IsUp('A'))
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Idle1", 17);
		}
		if (InputManager::IsPress('D') && pSkeletalMeshComponent_->GetCurrentAnimationKey() != "AM_Werewolf_Walk")
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Walk", 16);
		}
		if (InputManager::IsUp('D'))
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Idle1", 17);
		}
		if (InputManager::IsDown('F'))
		{
			pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Attack", 18);
		}
	}
}

void WereWolf::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pBodyMaterial;
	IMaterial* pFurMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, "MS_Werewolf", 11);
	MaterialManager::Instance()->GetMaterial(&pBodyMaterial, "MT_Werewolf_Body", 16);
	MaterialManager::Instance()->GetMaterial(&pFurMaterial, "MT_Werewolf_Fur", 15);
	pSkeletalMeshComponent_->InitPSO(1, 2, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pSkeletalMeshComponent_->GetPSO()->SetMeshToSlot(0, pMesh);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(0, pBodyMaterial);
	pSkeletalMeshComponent_->GetPSO()->SetMaterialToSlot(1, pFurMaterial);
	pSkeletalMeshComponent_->SetSkeleton("SK_Werewolf", 11);
	pSkeletalMeshComponent_->ChangeAnimation("AM_Werewolf_Idle1", 17);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });

	pCameraComponent_->GetComponentTransform().SetPosition({ -4.0f, 0.0f, 3.1f, 1.0f });
	pCameraComponent_->GetComponentTransform().SetRotation({ 0.0f, 25.0f, 0.0f, 0.0f });
}

void WereWolf::Render()
{
	Player::Render();
}

void WereWolf::CleanUp()
{
}
