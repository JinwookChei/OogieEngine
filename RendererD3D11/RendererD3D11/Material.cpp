#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"


Material::Material()
	: refCount_(1)
	, shaderType_(E_SHADER_PRESET::DeferredSimple)
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

//bool Material::Init(float shineness,Float3 specularColor)
//{
//	shineness_ = shineness;
//	specularColor_ = specularColor;
//	
//	return true;
//}

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

//void __stdcall Material::SetTextures(unsigned int texNum, ITexture** ppTextures)
//{
//	if (texNum > 0)
//	{
//		if (nullptr == ppTextures)
//		{
//			DEBUG_BREAK();
//			return;
//		}
//	}
//
//	CleanTextures();
//
//	texturesNum_ = texNum;
//	ppTextures_ = (Texture**)ppTextures;
//	for (int i = 0; i < texturesNum_; ++i)
//	{
//		ppTextures_[i]->AddRef();
//	}
//}

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


	//for (int texIdx = 0; texIdx < texturesNum_; ++texIdx)
	//{
	//	ppTextures_[texIdx]->Bind(texIdx);
	//}
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
