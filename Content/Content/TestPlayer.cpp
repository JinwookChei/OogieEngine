#include "stdafx.h"
#include "TestPlayer.h"

TestPlayer::TestPlayer()
{
}

TestPlayer::~TestPlayer()
{
	CleanUp();
}

void TestPlayer::Tick(double deltaTime)
{
}

void TestPlayer::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 4);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 2);
	pRenderer_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pRenderer_->GetPSO()->SetMeshToSlot(0, pMesh);
	pRenderer_->GetPSO()->SetMaterialToSlot(0, pMaterial);


	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void TestPlayer::Render()
{
	pRenderer_->Render();
}

void TestPlayer::CleanUp()
{
}
