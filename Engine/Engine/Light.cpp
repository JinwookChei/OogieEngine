#include "stdafx.h"
#include "StaticMeshComponent.h"
#include "Light.h"

IPSO* Light::GLightPSO = nullptr;

Light::Light()
	: pStaticMesh_(nullptr)
	, diffuseColor_(1.0f, 1.0f, 1.0f, 1.0f)
	, specularColor_(0.8f, 0.8f, 0.8f, 1.0f)
	, ambientColor_(0.3f, 0.3f, 0.3f, 1.0f)
{
	if (nullptr == GLightPSO)
	{
		PipelineStateDesc psoDesc;
		IMesh* pLightPSOMesh = nullptr;
		IMaterial* pLightPSOMaterial = nullptr;
		if (false == MeshManager::Instance()->GetMesh(&pLightPSOMesh, 0)) DEBUG_BREAK();
		if (false == MaterialManager::Instance()->GetMaterial(&pLightPSOMaterial, 0)) DEBUG_BREAK();
		psoDesc.meshSlotCount = 1;
		psoDesc.materialSlotCount = 1;
		psoDesc.depthState = E_DEPTH_PRESET::DEPTH_DISABLE;
		psoDesc.rasterizerMode = E_RASTERIZER_PRESET::DISABLE;
		GLightPSO = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
		GLightPSO->SetMeshToSlot(0, pLightPSOMesh);
		GLightPSO->SetMaterialToSlot(0, pLightPSOMaterial);
	}
	else
	{
		GLightPSO->AddRef();
	}

	pStaticMesh_ = CreateComponent<StaticMeshComponent>();
}

Light::~Light()
{
	CleanUp();

	if (nullptr != GLightPSO)
	{
		GLightPSO->Release();
	}
}

void Light::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void Light::BeginPlay()
{
	IMesh* pMesh;
	IMaterial* pMaterial;
	MeshManager::Instance()->GetMesh(&pMesh, 12);
	MaterialManager::Instance()->GetMaterial(&pMaterial, 9);
	pStaticMesh_->InitPSO(1, 1, E_DEPTH_PRESET::DEPTH_ENABLE_WRITE, E_RASTERIZER_PRESET::SOLID);
	pStaticMesh_->GetPSO()->SetMeshToSlot(0, pMesh);
	pStaticMesh_->GetPSO()->SetMaterialToSlot(0, pMaterial);

	//pTransform_->SetScale({ 0.5f, 0.5f ,0.5f ,0.0f });
}

void Light::Render()
{
	Actor::Render();
	Renderer::Instance()->UpdateLightFrame(GetData());
	Renderer::Instance()->Render(Light::GLightPSO);
}

const Color& Light::DiffuseColor() const
{
	return diffuseColor_;
}

const Color& Light::SpecularColor() const
{
	return specularColor_;
}

const Color& Light::AmbientColor() const
{
	return ambientColor_;
}

void Light::CleanUp()
{
}
