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
		if (false == MeshManager::Instance()->GetMesh(&psoDesc.pMesh, 0)) DEBUG_BREAK();
		if (false == MaterialManager::Instance()->GetMaterial(&psoDesc.pMaterial, 0)) DEBUG_BREAK();
		psoDesc.blendState = E_BLEND_PRESET::ADDITIVE_BLEND;
		psoDesc.depthState = E_DEPTH_PRESET::DEPTH_DISABLE;
		psoDesc.samplerState = E_SAMPLER_PRESET::LINEAR_CLAMP;
		psoDesc.rasterizerMode = E_RASTERIZER_PRESET::DISABLE;
		GLightPSO = Renderer::GetFactory()->CreatePipelineStateObject(psoDesc);
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
