#include "stdafx.h"
#include "Material.h"

Material::Material()
	: pMaterialImpl_(nullptr)
{
}

Material::Material(IMaterial* pMaterial)
	: pMaterialImpl_(nullptr)
{
	SetMaterial(pMaterial);
}

Material::~Material()
{
	CleanUp();
}

void Material::SetVertexShader(IShader* vertexShader)
{
	if (nullptr == pMaterialImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pMaterialImpl_->SetVertexShader(vertexShader);
}

void Material::SetPixelShader(IShader* pixelShader)
{
	if (nullptr == pMaterialImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pMaterialImpl_->SetPixelShader(pixelShader);
}

void Material::SetSampler(ISamplerState* sampler, uint32_t slot/* = 0*/)
{
	if (nullptr == pMaterialImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pMaterialImpl_->SetSampler(sampler, slot);
}

void Material::Setting()
{
	if (nullptr == pMaterialImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pMaterialImpl_->Setting();
}

void Material::SetMaterial(IMaterial* pMaterial)
{
	if (nullptr != pMaterialImpl_)
	{
		pMaterialImpl_->Release();
		pMaterialImpl_ = nullptr;
	}
	if (nullptr != pMaterial)
	{
		pMaterialImpl_ = pMaterial;
		pMaterialImpl_->AddRef();
	}
}

void Material::CleanUp()
{
	if (nullptr != pMaterialImpl_)
	{
		pMaterialImpl_->Release();
		pMaterialImpl_ = nullptr;
	}
}
