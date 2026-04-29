#include "stdafx.h"
#include "House.h"

House::House()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

House::~House()
{
	CleanUp();
}

void House::BeginPlay()
{
	IMesh* pMesh;
	MeshManager::Instance()->GetMesh(&pMesh, 15);

	IMaterial* pDecalsMaterial;
	IMaterial* pWoodTrimMaterial;
	IMaterial* pRoofTileMaterial;
	IMaterial* pWoodEndMaterial;
	IMaterial* pWallPlasterMaterial;
	IMaterial* pMetalMaterial;
	IMaterial* pWallStoneMaterial;
	IMaterial* pGlassWindowMaterial;
	IMaterial* pTransparentMaterial;
	MaterialManager::Instance()->GetMaterial(&pDecalsMaterial, 100);
	MaterialManager::Instance()->GetMaterial(&pWoodTrimMaterial, 101);
	MaterialManager::Instance()->GetMaterial(&pRoofTileMaterial, 102);
	MaterialManager::Instance()->GetMaterial(&pWoodEndMaterial, 103);
	MaterialManager::Instance()->GetMaterial(&pWallPlasterMaterial, 104);
	MaterialManager::Instance()->GetMaterial(&pMetalMaterial, 105);
	MaterialManager::Instance()->GetMaterial(&pWallStoneMaterial, 106);
	MaterialManager::Instance()->GetMaterial(&pGlassWindowMaterial, 107);
	MaterialManager::Instance()->GetMaterial(&pTransparentMaterial, 108);
	pStaticMesh_->InitPSO(1, 10, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pDecalsMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(1, pWoodTrimMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(2, pRoofTileMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(3, pWoodEndMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(4, pWallPlasterMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(5, pMetalMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(6, pWallStoneMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(7, pGlassWindowMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(8, pTransparentMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(9, pTransparentMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void House::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void House::Render()
{
	Actor::Render();
}

void House::CleanUp()
{
}
