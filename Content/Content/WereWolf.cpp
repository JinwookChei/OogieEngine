#include "stdafx.h"
#include "WereWolf.h"

WereWolf::WereWolf()
{
}

WereWolf::~WereWolf()
{
	CleanUp();
}

void WereWolf::Tick(double deltaTime)
{
}

void WereWolf::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial1;
	IMaterial* pMaterial2;
	MeshManager::Instance()->GetMesh(&pMesh, 5);
	MaterialManager::Instance()->GetMaterial(&pMaterial1, 4);
	MaterialManager::Instance()->GetMaterial(&pMaterial2, 5);
	pRenderer_->InitPSO(1, 2, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pRenderer_->GetPSO()->SetMeshToSlot(0, pMesh);
	pRenderer_->GetPSO()->SetMaterialToSlot(0, pMaterial1);
	pRenderer_->GetPSO()->SetMaterialToSlot(1, pMaterial2);


	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void WereWolf::Render()
{
	pRenderer_->Render();
}

void WereWolf::CleanUp()
{
}
