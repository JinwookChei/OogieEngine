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




SamplerStateT::SamplerStateT()
	: refCount_(1)
	, pSamplerState_(nullptr)
{
}

SamplerStateT::~SamplerStateT()
{
	CleanUp();
}

bool SamplerStateT::Init(const E_SAMPLER_TYPE_T& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy)
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.MinLOD = (FLOAT)minLOD;
	samplerDesc.MaxLOD = (FLOAT)maxLOD;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	

	switch (samplerType)
	{
	case E_SAMPLER_TYPE_T::LINEAR_WARP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = 1;
	}break;
	case E_SAMPLER_TYPE_T::LINEAR_CLAMP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.MaxAnisotropy = 1;
		
	}break;
	case E_SAMPLER_TYPE_T::ANISOTROPIC_WARP:
	{
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = (UINT)maxAnisotropy;

	}break;
	case E_SAMPLER_TYPE_T::ANISOTROPIC_CLAMP:
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

HRESULT __stdcall SamplerStateT::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall SamplerStateT::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall SamplerStateT::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

SamplerStateT* SamplerStateT::Create(const E_SAMPLER_TYPE_T& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy)
{
	SamplerStateT* pNewSamplerState = new SamplerStateT;
	if (false == pNewSamplerState->Init(samplerType, minLOD, maxLOD, maxAnisotropy))
	{
		pNewSamplerState->Release();
		pNewSamplerState = nullptr;
	}

	return pNewSamplerState;
}

void SamplerStateT::BindPS(UINT slot)
{
	GRenderer->DeviceContext()->PSSetSamplers(slot, 1, &pSamplerState_);
}

void SamplerStateT::CleanUp()
{
	if (nullptr != pSamplerState_)
	{
		pSamplerState_->Release();
		pSamplerState_ = nullptr;
	}
}