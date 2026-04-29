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
	IMaterial* pDecalsMaterial;
	IMaterial* pWoodTrimsMaterial;
	IMaterial* pRoofTileMaterial;
	IMaterial* pWoodEndMaterial;
	IMaterial* pWallPlasterMaterial;
	IMaterial* pMetalMaterial;
	IMaterial* pWallStoneMaterial;
	IMaterial* pGlassWindowMaterial;
	IMaterial* pTransparentMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, "MS_House", 8);
	MaterialManager::Instance()->GetMaterial(&pDecalsMaterial, "MT_House_Decals", 15);
	MaterialManager::Instance()->GetMaterial(&pWoodTrimsMaterial, "MT_House_WoodTrims", 18);
	MaterialManager::Instance()->GetMaterial(&pRoofTileMaterial, "MT_House_RoofTile", 17);
	MaterialManager::Instance()->GetMaterial(&pWoodEndMaterial, "MT_House_WoodEnd", 16);
	MaterialManager::Instance()->GetMaterial(&pWallPlasterMaterial, "MT_House_WallPlaster", 20);
	MaterialManager::Instance()->GetMaterial(&pMetalMaterial, "MT_House_Metal", 14);
	MaterialManager::Instance()->GetMaterial(&pWallStoneMaterial, "MT_House_WallStone", 18);
	MaterialManager::Instance()->GetMaterial(&pGlassWindowMaterial, "MT_House_GlassWindow", 20);
	MaterialManager::Instance()->GetMaterial(&pTransparentMaterial, "MT_House_Transparent", 20);
	pStaticMesh_->InitPSO(1, 10, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pDecalsMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(1, pWoodTrimsMaterial);
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
