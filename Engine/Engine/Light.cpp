#include "stdafx.h"
#include "StaticMeshComponent.h"
#include "Light.h"

IPSO* Light::GLightPSO = nullptr;
IPSO* Light::GAmbientPSO = nullptr;

Light::Light()
	: pStaticMesh_(nullptr)
	, intensity_ (1.0f)
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

	if (nullptr == GAmbientPSO)
	{
		PipelineStateDesc psoDesc;
		IMesh* pAmbientPSOMesh = nullptr;
		IMaterial* pAmbientPSOMaterial = nullptr;
		if (false == MeshManager::Instance()->GetMesh(&pAmbientPSOMesh, 0)) DEBUG_BREAK();
		if (false == MaterialManager::Instance()->GetMaterial(&pAmbientPSOMaterial, 4)) DEBUG_BREAK();
		psoDesc.meshSlotCount = 1;
		psoDesc.materialSlotCount = 1;
		psoDesc.depthState = E_DEPTH_PRESET::DEPTH_DISABLE;
		psoDesc.rasterizerMode = E_RASTERIZER_PRESET::DISABLE;
		GAmbientPSO = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
		GAmbientPSO->SetMeshToSlot(0, pAmbientPSOMesh);
		GAmbientPSO->SetMaterialToSlot(0, pAmbientPSOMaterial);
	}
	else
	{
		GAmbientPSO->AddRef();
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

	if (nullptr != GAmbientPSO)
	{
		GAmbientPSO->Release();
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
}

void Light::Render()
{
	Actor::Render();
}

void Light::RenderLight()
{
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

Color __stdcall Light::GetDiffuseColorForEditor() const
{
	return diffuseColor_;
}

Color __stdcall Light::GetSpecularColorForEditor() const
{
	return specularColor_;
}

Color __stdcall Light::GetAmbientColorForEditor() const
{
	return ambientColor_;
}

void __stdcall Light::SetDiffuseColorForEditor(const Color& color)
{
	diffuseColor_ = color;
}

void __stdcall Light::SetSpecularColorForEditor(const Color& color)
{
	specularColor_ = color;
}

void __stdcall Light::SetAmbientColorForEditor(const Color& color)
{
	ambientColor_ = color;
}

float __stdcall Light::GetLightIntensity() const
{
	return intensity_;
}

void __stdcall Light::SetLightIntensity(float intensity)
{
	intensity_ = intensity;
}

IMaterial* __stdcall Light::GetMaterialForEditor() const
{
	return pStaticMesh_->GetPSO()->GetMaterial(0);
}
