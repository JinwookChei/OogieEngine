#include "stdafx.h"
#include "ConstantManager.h"

ConstantManager::ConstantManager()
{
	pPerFrameBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerFrame));
	if (nullptr == pPerFrameBuffer_)
	{
		Assert("CBPerFrame is NULL!");
		return;
	}

	//pPerMergeFrameBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerMergeFrame));
	//if (nullptr == pPerMergeFrameBuffer_)
	//{
	//	Assert("PerMergeFrameBuffer_ is NULL!");
	//	return;
	//}

	pPerObjectBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerObject));
	if (nullptr == pPerObjectBuffer_)
	{
		Assert("PerObjectBuffer is NULL!");
		return;
	}

	pPerLightBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerLight));
	if (nullptr == pPerLightBuffer_)
	{
		Assert("PerLightBuffer_ is NULL!");
		return;
	}
}

ConstantManager::~ConstantManager()
{
	CleanUp();
}

ConstantManager* ConstantManager::Instance()
{
	return GConstantManager;
}

void ConstantManager::UpdatePerFrame(CBPerFrame* pCBPerFrame)
{
	pPerFrameBuffer_->Update(pCBPerFrame);
	pPerFrameBuffer_->VSSetting(0);
	pPerFrameBuffer_->PSSetting(0);
}


//void ConstantManager::UpdatePerMergeFrame(CBPerMergeFrame* pCBPerMergeFrame)
//{
//	pPerMergeFrameBuffer_->Update(pCBPerMergeFrame);
//	pPerMergeFrameBuffer_->VSSetting(0);
//	pPerMergeFrameBuffer_->PSSetting(0);
//}


void ConstantManager::UpdatePerObejct(CBPerObject* cbPerObject)
{
	pPerObjectBuffer_->Update(cbPerObject);
	pPerObjectBuffer_->VSSetting(1);
	pPerObjectBuffer_->PSSetting(1);
}

void ConstantManager::UpdatePerLight(CBPerLight* pCBPerLight)
{
	pPerLightBuffer_->Update(pCBPerLight);
	pPerLightBuffer_->PSSetting(1);
}


void ConstantManager::CleanUp()
{
	if (nullptr != pPerFrameBuffer_)
	{
		pPerFrameBuffer_->Release();
		pPerFrameBuffer_ = nullptr;
	}

	//if (nullptr != pPerMergeFrameBuffer_)
	//{
	//	pPerMergeFrameBuffer_->Release();
	//	pPerMergeFrameBuffer_ = nullptr;
	//}

	if (nullptr != pPerObjectBuffer_)
	{
		pPerObjectBuffer_->Release();
		pPerObjectBuffer_ = nullptr;
	}

	if (nullptr != pPerLightBuffer_)
	{
		pPerLightBuffer_->Release();
		pPerLightBuffer_ = nullptr;
	}
}
