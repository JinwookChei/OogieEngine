#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"


Material::Material()
	: refCount_(1)
	, shaderType_(E_SHADER_PRESET::DeferredSimple)
	, textureCount_(0)
	, ppTextures_(nullptr)
	//, pTextureAlbedo_(nullptr)
	//, pTextureNormal_(nullptr)
	, shineness_(1.0f)
	, specularColor_(0.7f, 0.7f, 0.7f)
{
}

Material::~Material()
{
	CleanUp();
}

bool Material::Init(float shineness,Float3 specularColor)
{
	shineness_ = shineness;
	specularColor_ = specularColor;
	
	return true;
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
	//if (nullptr == desc.pShader)
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}
	//if (nullptr == desc.pTextureAlbedo)
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}
	//if (nullptr == desc.pTextureNormal)
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	//pShader_ = desc.pShader;
	//pShader_->AddRef();

	shaderType_ = desc.shaderType;

	textureCount_ = desc.textureCount_;
	if (textureCount_ > 0)
	{
		if (nullptr == desc.ppTextures_)
		{
			DEBUG_BREAK();
			return false;
		}
	}

	ppTextures_ = (Texture**)desc.ppTextures_;
	for (int i = 0; i < textureCount_; ++i)
	{
		ppTextures_[i]->AddRef();
	}
	

	/*pTextureAlbedo_ = static_cast<Texture*>(desc.pTextureAlbedo);
	pTextureAlbedo_->AddRef();

	pTextureNormal_ = static_cast<Texture*>(desc.pTextureNormal);
	pTextureNormal_->AddRef();*/




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
	if (0 == refCount_) {
		delete this;
	}
	return tmpRefCount;
}

//void __stdcall Material::Setting()
//{
//
//}

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
	case E_SHADER_PRESET::DeferredSimple: 
	{
		Shader::GShaderDeferredSimple->Bind();
		break;
	}
	default:
		DEBUG_BREAK();
		break;
	}
	

	for (int texIdx = 0; texIdx < textureCount_; ++texIdx)
	{
		ppTextures_[texIdx]->Bind(texIdx);
	}
	//pTextureAlbedo_->Bind(0);
	//pTextureNormal_->Bind(1);
}

void Material::CleanUp()
{
	//if (nullptr != pShader_)
	//{
	//	pShader_->Release();
	//	pShader_ = nullptr;
	//}

	for (int i = 0; i < textureCount_; ++i)
	{
		if (nullptr != ppTextures_[i])
		{
			ppTextures_[i]->Release();
			ppTextures_[i] = nullptr;
		}
	}

	if(nullptr != ppTextures_)
	{
		delete[] ppTextures_;
		ppTextures_ = nullptr;
	}


	//if (nullptr != pTextureAlbedo_)
	//{
	//	pTextureAlbedo_->Release();
	//	pTextureAlbedo_ = nullptr;
	//}
	//if (nullptr != pTextureNormal_)
	//{
	//	pTextureNormal_->Release();
	//	pTextureNormal_ = nullptr;
	//}
}
