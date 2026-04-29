#include "stdafx.h"
#include "BricksCube.h"

BricksCube::BricksCube()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

BricksCube::~BricksCube()
{
	CleanUp();
}

void BricksCube::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 10);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 10);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void BricksCube::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void BricksCube::Render()
{
	Actor::Render();
}

void BricksCube::CleanUp()
{
}

IMaterial* __stdcall BricksCube::GetMaterialForEditor() const
{
	return pStaticMesh_->GetPSO()->GetMaterial(0);
}
