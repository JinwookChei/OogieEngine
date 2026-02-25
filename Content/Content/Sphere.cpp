#include "stdafx.h"
#include "Sphere.h"

Sphere::Sphere()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

Sphere::~Sphere()
{
	CleanUp();
}

void Sphere::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 11);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 10);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Sphere::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void Sphere::Render()
{
	Actor::Render();
}

void Sphere::CleanUp()
{
}
