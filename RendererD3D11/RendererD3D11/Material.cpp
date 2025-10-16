#include "stdafx.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "Material.h"


Material::Material(VertexShader* pVertexShader, PixelShader* pPixelShader, SamplerState* pSamplerState)
	: refCount_(1),
	pVertexShader_(pVertexShader),
	pPixelShader_(pPixelShader),
	pSamplerState_(pSamplerState)
{
}

Material::~Material()
{
	CleanUp();
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

	pSamplerState_->Setting(0);
}

IShader* __stdcall Material::GetVertexShader()
{
	return pVertexShader_;
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
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
}
