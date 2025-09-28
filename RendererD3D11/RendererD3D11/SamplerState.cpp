#include "stdafx.h"
#include "SamplerState.h"

D3D11SamplerState::D3D11SamplerState()
	: refCount_(1),
	pSamplerState_(nullptr)
{
}

D3D11SamplerState::~D3D11SamplerState()
{
	CleanUp();
}

HRESULT __stdcall D3D11SamplerState::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11SamplerState::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11SamplerState::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall D3D11SamplerState::Setting(uint32_t slot)
{
	GRenderer->DeviceContext()->PSSetSamplers(slot, 1, &pSamplerState_);
}

bool D3D11SamplerState::CreateSampler(bool linear, bool clamp)
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

	HRESULT hr = GRenderer->Device()->CreateSamplerState(&samplerDesc, &pSamplerState_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "SamplerState::pSamplerState_";
	pSamplerState_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}


void D3D11SamplerState::CleanUp()
{
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
}
