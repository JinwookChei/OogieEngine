#include "stdafx.h"
#include "Tree.h"

Tree::Tree()
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

Tree::~Tree()
{
	CleanUp();
}

void Tree::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pTrunkAMaterial;
	IMaterial* pTrunkBMaterial;
	IMaterial* pTwoSidedMaterial;
	IMaterial* pDecorationMaterial;

	MeshManager::Instance()->GetMesh(&pMesh, "MS_Tree", 7);
	MaterialManager::Instance()->GetMaterial(&pTrunkAMaterial, "MT_Tree_TrunkA", 14);
	MaterialManager::Instance()->GetMaterial(&pTrunkBMaterial, "MT_Tree_TrunkB", 14);
	MaterialManager::Instance()->GetMaterial(&pTwoSidedMaterial, "MT_Tree_TwoSided", 16);
	MaterialManager::Instance()->GetMaterial(&pDecorationMaterial, "MT_Tree_Decoration", 18);
	
	pStaticMesh_->InitPSO(1, 4, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pTrunkAMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(1, pTrunkBMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(2, pTwoSidedMaterial);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(3, pDecorationMaterial);

	pTransform_->SetScale({ 1.0f, 1.0f, 1.0f, 0.0f });
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f });
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void Tree::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void Tree::Render()
{
	Actor::Render();
}

void Tree::CleanUp()
{
}
