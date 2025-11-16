#include "stdafx.h"
#include "SamplerManager.h"


SamplerManager::SamplerManager()
	:ppCurSampler_(nullptr)
	, pLinearClamp_(nullptr)
	, pLinearWarp_(nullptr)
	, pAnisotropicClamp_(nullptr)
	, pAnisotropicWarp_(nullptr)
{
	SamplerStateDesc samplerDesc;
	samplerDesc.filter = 21;
	samplerDesc.addressMethod = 3;
	pLinearClamp_ = GRenderer->CreateSamplerState(samplerDesc);
	if (nullptr == pLinearClamp_)
	{
		DEBUG_BREAK();
		return;
	}

	samplerDesc.filter = 21;
	samplerDesc.addressMethod = 1;
	pLinearWarp_ = GRenderer->CreateSamplerState(samplerDesc);
	if (nullptr == pLinearWarp_)
	{
		DEBUG_BREAK();
		return;
	}

	samplerDesc.filter = 85;
	samplerDesc.addressMethod = 3;
	pAnisotropicClamp_ = GRenderer->CreateSamplerState(samplerDesc);
	if (nullptr == pAnisotropicClamp_)
	{
		DEBUG_BREAK();
		return;
	}

	samplerDesc.filter = 85;
	samplerDesc.addressMethod = 1;
	pAnisotropicWarp_ = GRenderer->CreateSamplerState(samplerDesc);
	if (nullptr == pAnisotropicWarp_)
	{
		DEBUG_BREAK();
		return;
	}

	ppCurSampler_ = &pLinearClamp_;
	(*ppCurSampler_)->AddRef();
	(*ppCurSampler_)->Setting(0);
}


SamplerManager::~SamplerManager()
{
	CleanUp();
}

SamplerManager* SamplerManager::Instance()
{
	return GSamplerManager;
}

void SamplerManager::Setting(const E_SAMPLER_MODE& samplerMode)
{
	switch (samplerMode)
	{
	case E_SAMPLER_MODE::LINEAR_CLAMP:
	{
		if (ppCurSampler_ == &pLinearClamp_) return;
		(*ppCurSampler_)->Release();
		ppCurSampler_ = &pLinearClamp_;
		(*ppCurSampler_)->AddRef();
	}break;
	case E_SAMPLER_MODE::LINEAR_WARP:
	{
		if (ppCurSampler_ == &pLinearWarp_) return;
		(*ppCurSampler_)->Release();
		ppCurSampler_ = &pLinearWarp_;
		(*ppCurSampler_)->AddRef();
	}break;
	case E_SAMPLER_MODE::ANISOTROPIC_CLAMP:
	{
		if (ppCurSampler_ == &pAnisotropicClamp_) return;
		(*ppCurSampler_)->Release();
		ppCurSampler_ = &pAnisotropicClamp_;
		(*ppCurSampler_)->AddRef();
	}break;
	case E_SAMPLER_MODE::ANISOTROPIC_WARP:
	{
		if (ppCurSampler_ == &pAnisotropicWarp_) return;
		(*ppCurSampler_)->Release();
		ppCurSampler_ = &pAnisotropicWarp_;
		(*ppCurSampler_)->AddRef();
	}break;
	default:
		break;
	}

	(*ppCurSampler_)->Setting(0);
}


void SamplerManager::CleanUp()
{
	if (nullptr != *ppCurSampler_)
	{
		(*ppCurSampler_)->Release();
		ppCurSampler_ = nullptr;
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

