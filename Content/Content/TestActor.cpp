#include "stdafx.h"
#include "TestActor.h"

TestActor::TestActor()
	//: pStaticMesh_ (new StaticMeshComponent(this))
{
	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

TestActor::~TestActor()
{
	CleanUp();
}

void TestActor::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 10);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 15);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	pTransform_->SetScale({1.0f, 1.0f, 1.0f, 0.0f});
	pTransform_->SetRotation({ 0.0f, 0.0f, 0.0f, 0.0f});
	pTransform_->SetPosition({ 0.0f, 0.0f, 0.0f, 1.0f });
}

void TestActor::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void TestActor::Render()
{
	Actor::Render();
}

void TestActor::CleanUp()
{
}
