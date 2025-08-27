#include "stdafx.h"
#include "Material.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"

Material::Material()
	: refCount_(1),
	pVertexShader_(nullptr),
	pPixelShader_(nullptr),
	pSamplerState_(nullptr),
	samplerSlot_(0)
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

void __stdcall Material::SetVertexShader(IShader* pVertexShader)
{
	if (nullptr != pVertexShader_)
	{
		pVertexShader_->Release();
		pVertexShader_ = nullptr;
	}

	pVertexShader_ = (VertexShader*)pVertexShader;

	if (nullptr != pVertexShader_)
	{
		pVertexShader_->AddRef();
	}
}

void __stdcall Material::SetPixelShader(IShader* pPixelShader)
{
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->Release();
		pPixelShader_ = nullptr;
	}

	pPixelShader_ = (PixelShader*)pPixelShader;

	if (nullptr != pPixelShader_)
	{
		pPixelShader_->AddRef();
	}
}

void __stdcall Material::SetSampler(ISamplerState* pSamplerState, uint32_t slot)
{
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}

	pSamplerState_ = (SamplerState*)pSamplerState;

	if (nullptr != pSamplerState_)
	{
		pSamplerState_->AddRef();
	}

	samplerSlot_ = slot;
}

void __stdcall Material::Setting()
{
	if (nullptr != pVertexShader_)
	{
		pVertexShader_->SetShader();
	}
	if (nullptr != pPixelShader_)
	{
		pPixelShader_->SetShader();
	}
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Setting(samplerSlot_);
	}
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
