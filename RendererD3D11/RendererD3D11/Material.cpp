#include "SamplerManager.h"
#include "stdafx.h"
#include "VertexShader.h"
#include "PixelShader.h"
//#include "SamplerState.h"
#include "Material.h"


Material::Material()
	: refCount_(1),
	pVertexShader_(nullptr),
	pPixelShader_(nullptr),
	//pSamplerState_(nullptr),
	shineness_(1.0f),
	specularColor_(0.7f, 0.7f, 0.7f)
{
}

Material::~Material()
{
	CleanUp();
}

bool Material::Init(
	VertexShader* pVertexShader, 
	PixelShader* pPixelShader, 
	/*SamplerState* pSamplerState,*/
	float shineness,
	Float3 specularColor
)
{
	if (nullptr != pVertexShader)
	{
		pVertexShader_ = pVertexShader;
	}
	if (nullptr != pPixelShader)
	{
		pPixelShader_ = pPixelShader;
	}
	//if (nullptr != pSamplerState)
	//{
	//	pSamplerState_ = pSamplerState;
	//}

	shineness_ = shineness;
	specularColor_ = specularColor;
	
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

void __stdcall Material::Setting()
{
	pVertexShader_->Setting();

	pPixelShader_->Setting();

	//pSamplerState_->Setting(0);
}

IShader* __stdcall Material::GetVertexShader()
{
	return pVertexShader_;
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

void Material::CleanUp()
{
	if (nullptr != pVertexShader_)
	{
		pVertexShader_->Release();
		pVertexShader_ = nullptr;
	}
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->Release();
		pPixelShader_ = nullptr;
	}
	//if (nullptr != pSamplerState_)
	//{
	//	pSamplerState_->Release();
	//	pSamplerState_ = nullptr;
	//}
}
