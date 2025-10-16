#include "stdafx.h"
#include "SamplerState.h"

SamplerState::SamplerState(ID3D11SamplerState* pSamplerState)
	: refCount_(1),
	pSamplerState_(pSamplerState)
{
}

SamplerState::~SamplerState()
{
	CleanUp();
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
	GRenderer->DeviceContext()->PSSetSamplers(slot, 1, &pSamplerState_);
}

void SamplerState::CleanUp()
{
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
}
