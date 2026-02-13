#include "stdafx.h"
#include "Wukong.h"

Wukong::Wukong()
{
}

Wukong::~Wukong()
{
	CleanUp();
}

void Wukong::Tick(double deltaTime)
{
}

void Wukong::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 3);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 3);
	pRenderer_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pRenderer_->GetPSO()->SetMeshToSlot(0, pMesh);
	pRenderer_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Wukong::Render()
{
	pRenderer_->Render();
}

void Wukong::CleanUp()
{
}
