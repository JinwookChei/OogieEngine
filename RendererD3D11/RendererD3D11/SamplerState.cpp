#include "stdafx.h"
#include "SamplerState.h"

SamplerState::SamplerState()
	: refCount_(1)
	, pSamplerState_(nullptr)
{
}

SamplerState::~SamplerState()
{
	CleanUp();
}

bool SamplerState::Init(const E_SAMPLER_TYPE& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.MinLOD = (FLOAT)minLOD;
	samplerDesc.MaxLOD = (FLOAT)maxLOD;
	samplerDesc.MipLODBias = 0.0f;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	

	switch (samplerType)
	{
	case E_SAMPLER_TYPE::LINEAR_WARP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = 1;
	}break;
	case E_SAMPLER_TYPE::LINEAR_CLAMP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MaxAnisotropy = 1;
		
	}break;
	case E_SAMPLER_TYPE::ANISOTROPIC_WARP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = (UINT)maxAnisotropy;

	}break;
	case E_SAMPLER_TYPE::ANISOTROPIC_CLAMP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MaxAnisotropy = (UINT)maxAnisotropy;
	}break;

	default:
		break;
	}

	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateSamplerState(&samplerDesc, &pSamplerState_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}
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

SamplerState* SamplerState::Create(const E_SAMPLER_TYPE& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy)
{
	SamplerState* pNewSamplerState = new SamplerState;
	if (false == pNewSamplerState->Init(samplerType, minLOD, maxLOD, maxAnisotropy))
	{
		DEBUG_BREAK();
		pNewSamplerState->Release();
		pNewSamplerState = nullptr;
	}

	return pNewSamplerState;
}

void SamplerState::BindPS(UINT slot)
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