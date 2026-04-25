#include "stdafx.h"
#include "ColorSphere.h"

ColorSphere::ColorSphere()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

ColorSphere::~ColorSphere()
{
	CleanUp();
}

void ColorSphere::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 11);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 17);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void ColorSphere::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void ColorSphere::Render()
{
	Actor::Render();
}

void ColorSphere::CleanUp()
{
}