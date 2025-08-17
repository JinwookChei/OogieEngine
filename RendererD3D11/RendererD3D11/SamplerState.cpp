#include "stdafx.h"
#include "SamplerState.h"

SamplerState::SamplerState()
	: refCount_(1),
	samplerState_(nullptr)
{
}

SamplerState::~SamplerState()
{
	Cleanup();
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
	GRenderer->DeviceContext()->PSSetSamplers(slot, 1, &samplerState_);
}

bool SamplerState::CreateSampler(bool linear, bool clamp)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = linear ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = clamp ? D3D11_TEXTURE_ADDRESS_CLAMP : D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = FLT_MIN;
	samplerDesc.MaxLOD = FLT_MAX;

	HRESULT hr = GRenderer->Device()->CreateSamplerState(&samplerDesc, &samplerState_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


void SamplerState::Cleanup()
{
	if (nullptr != samplerState_)
	{
		samplerState_->Release();
		samplerState_ = nullptr;
	}
}
