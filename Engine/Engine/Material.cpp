#include "stdafx.h"
#include "Material.h"

Material::Material()
	: pVertexShaderImpl_(nullptr),
	pPixelShaderImpl_(nullptr),
	pSampleStateImpl_(nullptr)
{
}

Material::Material(IShader* pVertexShader, IShader* pPixelShader, ISamplerState* pSampleState)
	:pVertexShaderImpl_(pVertexShader),
	pPixelShaderImpl_(pPixelShader),
	pSampleStateImpl_(pSampleState)
{

}

Material::~Material()
{
	CleanUp();
}

IShader* Material::GetVertexShader() const
{
	return pVertexShaderImpl_;
}

void Material::SetSampler(ISamplerState* sampler, uint32_t slot/* = 0*/)
{

}

void Material::Setting()
{
	pVertexShaderImpl_->Setting();

	pPixelShaderImpl_->Setting();
}

void Material::CleanUp()
{
	if (nullptr != pVertexShaderImpl_)
	{
		pVertexShaderImpl_->Release();
		pVertexShaderImpl_ = nullptr;
	}
	if (nullptr != pPixelShaderImpl_)
	{
		pPixelShaderImpl_->Release();
		pPixelShaderImpl_ = nullptr;
	}
	if (nullptr != pSampleStateImpl_)
	{
		pSampleStateImpl_->Release();
		pSampleStateImpl_ = nullptr;
	}
}
