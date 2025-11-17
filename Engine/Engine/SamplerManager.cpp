#include "stdafx.h"
#include "SamplerManager.h"


SamplerManager::SamplerManager()
	: pCurSampler_(nullptr)
{
	pCurSampler_ = GRenderer->CreateSamplerState(FLT_MIN, FLT_MAX, 1);
	if (nullptr == pCurSampler_)
	{
		Assert("CreateSamplerState is Fail");
		return;
	}
}


SamplerManager::~SamplerManager()
{
	CleanUp();
}

SamplerManager* SamplerManager::Instance()
{
	return GSamplerManager;
}

void SamplerManager::Setting(unsigned int slot, const E_SAMPLER_TYPE& samplerMode)
{
	pCurSampler_->ChangeSampler(samplerMode);

	pCurSampler_->Setting(slot);
}


void SamplerManager::CleanUp()
{
	if (nullptr != pCurSampler_)
	{
		pCurSampler_->Release();
		pCurSampler_ = nullptr;
	}	
}

