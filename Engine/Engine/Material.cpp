#include "stdafx.h"
#include "Material.h"


Material::Material(IShader* pVertexShader, IShader* pPixelShader, ISamplerState* pSamplerState)
	:pVertexShaderImpl_(pVertexShader),
	pPixelShaderImpl_(pPixelShader),
	pSamplerStateImpl_(pSamplerState)
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

//void Material::SetSampler(ISamplerState* sampler, uint32_t slot/* = 0*/)
//{
//
//}

void Material::Setting()
{
	pVertexShaderImpl_->Setting();

	pPixelShaderImpl_->Setting();

	pSamplerStateImpl_->Setting(0);
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
	if (nullptr != pSamplerStateImpl_)
	{
		pSamplerStateImpl_->Release();
		pSamplerStateImpl_ = nullptr;
	}
}
