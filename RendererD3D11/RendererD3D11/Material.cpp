#include "stdafx.h"
#include "Shader.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Texture.h"
#include "Material.h"


Material::Material()
	: refCount_(1)
	, shaderType_(E_SHADER_PRESET::DeferredSimple)
	, samplerState_(E_SAMPLER_PRESET::LINEAR_CLAMP)
	, blendState_(E_BLEND_PRESET::ALPHA_BLEND)
	, texturesNum_(0)
	, ppTextures_(nullptr)
	, shineness_(1.0f)
	, specularColor_(0.7f, 0.7f, 0.7f)
{
}

Material::~Material()
{
	CleanUp();
}

IMaterial* Material::Create(const MaterialDesc& desc)
{
	Material* pNewMaterial = new Material;
	if (false == pNewMaterial->Init(desc))
	{
		pNewMaterial->Release();
		pNewMaterial = nullptr;
		return nullptr;
	}

	return pNewMaterial;
}

bool Material::Init(const MaterialDesc& desc)
{
	shaderType_ = desc.shaderType;
	samplerState_ = desc.samplerState;
	blendState_ = desc.blendState;

	CleanTextures();
	texturesNum_ = desc.textureNum_;
	if(0 != texturesNum_)
	{
		ppTextures_ = new Texture * [desc.textureNum_]{nullptr};
	}
	shineness_ = desc.shineness;
	specularColor_ = desc.specularColor;
	return true;
}

HRESULT __stdcall Material::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Material::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Material::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) 
{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall Material::SetTextures(unsigned int texIdx, ITexture* pTexture)
{
	if (texIdx >= texturesNum_)
	{
		DEBUG_BREAK();
		return;
	}

	if (nullptr != ppTextures_[texIdx])
	{
		ppTextures_[texIdx]->Release();
		ppTextures_[texIdx] = nullptr;
	}

	ppTextures_[texIdx] = static_cast<Texture*>(pTexture);
	ppTextures_[texIdx]->AddRef();
}

float Material::GetShineness() const
{
	return shineness_;
}

void Material::SetShineness(float shineness)
{
	shineness_ = shineness;
}

const Float3& Material::GetSpecularColor() const
{
	return specularColor_;
}

void Material::SetSpecularColor(const Float3& specularColor)
{
	specularColor_ = specularColor;
}

const E_SAMPLER_PRESET& Material::GetSamplerState() const
{
	return samplerState_;
}

const E_BLEND_PRESET& Material::GetBlendState() const
{
	return blendState_;
}

void Material::Bind()
{
	switch (shaderType_)
	{
	case E_SHADER_PRESET::Light:
	{
		Shader::GShaderLight->Bind();
		break;
	}
	case E_SHADER_PRESET::DeferredSimple: 
	{
		Shader::GShaderDeferredSimple->Bind();
		break;
	}
	default:
		DEBUG_BREAK();
		break;
	}
	
	switch (samplerState_)
	{
	case E_SAMPLER_PRESET::LINEAR_CLAMP:
	{
		SamplerState::GSamplerLinearClamp->BindPS(0);
		break;
	}
	case E_SAMPLER_PRESET::LINEAR_WARP:
	{
		SamplerState::GSamplerLinearWarp->BindPS(0);
		break;
	}
	case E_SAMPLER_PRESET::ANISOTROPIC_CLAMP:
	{
		SamplerState::GSamplerAnisotropicClamp->BindPS(0);
		break;
	}
	case E_SAMPLER_PRESET::ANISOTROPIC_WARP:
	{
		SamplerState::GSamplerAnisotropicClamp->BindPS(0);
		break;
	}
	default:
	{
		DEBUG_BREAK();
		break;
	}
	}

	switch (blendState_)
	{
	case E_BLEND_PRESET::OPAQUE_BLEND:
	{
		BlendState::GOpaqueBlend->Bind();
		break;
	}
	case E_BLEND_PRESET::ALPHA_BLEND:
	{
		BlendState::GAlphaBlend->Bind();
		break;
	}
	case E_BLEND_PRESET::ADDITIVE_BLEND:
	{
		BlendState::GAdditiveBlend->Bind();
		break;
	}
	default:
	{
		DEBUG_BREAK();
		break;
	}
	}

	ID3D11ShaderResourceView* pSRVs[16]; 
	for (int i = 0; i < texturesNum_; ++i) 
	{
		pSRVs[i] = ppTextures_[i]->ShaderResourceView();
		if (nullptr == pSRVs[i])
		{
			DEBUG_BREAK();
		}
	}
	GRenderer->DeviceContext()->PSSetShaderResources(0, texturesNum_, pSRVs);
}

void Material::CleanTextures()
{
	for (int i = 0; i < texturesNum_; ++i)
	{
		if (nullptr != ppTextures_[i])
		{
			ppTextures_[i]->Release();
			ppTextures_[i] = nullptr;
		}
	}

	if (nullptr != ppTextures_)
	{
		delete[] ppTextures_;
		ppTextures_ = nullptr;
	}
}

void Material::CleanUp()
{
	CleanTextures();
}
