#include "stdafx.h"
#include "Floor.h"

Floor::Floor()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

Floor::~Floor()
{
	CleanUp();
}

void Floor::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 10);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 16);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Floor::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void Floor::Render()
{
	Actor::Render();
}

void Floor::CleanUp()
{
}
