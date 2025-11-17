#include "stdafx.h"
#include "BlendStateManager.h"


BlendStateManager::BlendStateManager()
	: pBlendState_(nullptr)
{
	pBlendState_ = GRenderer->CreateBlendState();
	if (nullptr == pBlendState_)
	{
		Assert("CreateBlendState is Fail!!");
		return;
	}
}

BlendStateManager::~BlendStateManager()
{
	CleanUp();
}

BlendStateManager* BlendStateManager::Instance()
{
	return GBlendStateManager;
}

void BlendStateManager::Clear()
{
	pBlendState_->Clear();
}

void BlendStateManager::Setting(const E_BLEND_MODE_TYPE& blendType)
{
	pBlendState_->ChangeBlendMode(blendType);
	pBlendState_->Setting();
}

void BlendStateManager::CleanUp()
{
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
}
