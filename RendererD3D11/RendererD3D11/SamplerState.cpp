#include "stdafx.h"
#include "SamplerState.h"

SamplerState::SamplerState()
	: refCount_(1)
	, pCurSampler_(nullptr)
	, pLinearClamp_(nullptr)
	, pLinearWarp_(nullptr)
	, pAnisotropicClamp_(nullptr)
	, pAnisotropicWarp_(nullptr)
{
}

SamplerState::~SamplerState()
{
	CleanUp();
}

bool SamplerState::Init
(
	ID3D11SamplerState* pLinearClamp,
	ID3D11SamplerState* pLinearWarp,
	ID3D11SamplerState* pAnisotropicClamp,
	ID3D11SamplerState* pAnisotropicWarp
)
{
	if (nullptr == pLinearClamp)
	{
		DEBUG_BREAK();
		return false;
	}
	if (nullptr == pLinearWarp)
	{
		DEBUG_BREAK();
		return false;
	}
	if (nullptr == pAnisotropicClamp)
	{
		DEBUG_BREAK();
		return false;
	}
	if (nullptr == pAnisotropicWarp)
	{
		DEBUG_BREAK();
		return false;
	}

	pLinearClamp_ = pLinearClamp;
	pLinearWarp_ = pLinearWarp;
	pAnisotropicClamp_ = pAnisotropicClamp;
	pAnisotropicWarp_ = pAnisotropicWarp;

	ChangeSampler(E_SAMPLER_TYPE::LINEAR_WARP);
	return true;
}

HRESULT __stdcall SamplerState::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall SamplerState::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall SamplerState::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall SamplerState::Setting(uint32_t slot)
{
	if (nullptr == pCurSampler_)
	{
		DEBUG_BREAK();
		return;
	}

	GRenderer->DeviceContext()->PSSetSamplers(slot, 1, &pCurSampler_);
}

void __stdcall SamplerState::ChangeSampler(E_SAMPLER_TYPE samplerType)
{
	if (nullptr != pCurSampler_)
	{
		pCurSampler_->Release();
		pCurSampler_ = nullptr;
	}

	switch (samplerType)
	{
	case E_SAMPLER_TYPE::LINEAR_CLAMP:
		pCurSampler_ = pLinearClamp_;
		pCurSampler_->AddRef();
		break;
	case E_SAMPLER_TYPE::LINEAR_WARP:
		pCurSampler_ = pLinearWarp_;
		pCurSampler_->AddRef();
		break;
	case E_SAMPLER_TYPE::ANISOTROPIC_CLAMP:
		pCurSampler_ = pAnisotropicClamp_;
		pCurSampler_->AddRef();
		break;
	case E_SAMPLER_TYPE::ANISOTROPIC_WARP:
		pCurSampler_ = pAnisotropicWarp_;
		pCurSampler_->AddRef();
		break;
	default:
		break;
	}
}

void SamplerState::CleanUp()
{
	if (nullptr != pCurSampler_)
	{
		pCurSampler_->Release();
		pCurSampler_ = nullptr;
	}
	if (nullptr != pLinearClamp_)
	{
		pLinearClamp_->Release();
		pLinearClamp_ = nullptr;
	}
	if (nullptr != pLinearWarp_)
	{
		pLinearWarp_->Release();
		pLinearWarp_ = nullptr;
	}
	if (nullptr != pAnisotropicClamp_)
	{
		pAnisotropicClamp_->Release();
		pAnisotropicClamp_ = nullptr;
	}
	if (nullptr != pAnisotropicWarp_)
	{
		pAnisotropicWarp_->Release();
		pAnisotropicWarp_ = nullptr;
	}
}
