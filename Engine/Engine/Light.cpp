#include "stdafx.h"
#include "Light.h"

IPSO* Light::GLightPSO = nullptr;

Light::Light()
	: diffuseColor_(1.0f, 1.0f, 1.0f, 1.0f)
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
}

void Light::BeginPlay()
{
}

void Light::Render()
{
	Renderer::Instance()->UpdateLightFrame(GetData());
	Renderer::Instance()->RenderTest(Light::GLightPSO);
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
