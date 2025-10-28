#include "stdafx.h"
#include "GraphicesStructures.h"
#include "ConstantManager.h"

ConstantManager::ConstantManager()
{
	pPerCameraFrameBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerCameraFrame));
	if (nullptr == pPerCameraFrameBuffer_)
	{
		Assert("PerCameraFrameBuffer is NULL!");
		return;
	}

	pPerMergeFrameBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerMergeFrame));
	if (nullptr == pPerMergeFrameBuffer_)
	{
		Assert("PerMergeFrameBuffer_ is NULL!");
		return;
	}

	pPerObjectBuffer_ = GRenderer->CreateConstantBuffer((uint32_t)sizeof(CBPerObject));
	if (nullptr == pPerObjectBuffer_)
	{
		Assert("PerObjectBuffer is NULL!");
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

void ConstantManager::UpdatePerCameraFrame(CBPerCameraFrame* pCBPerCameraFrame)
{
	pPerCameraFrameBuffer_->Update(pCBPerCameraFrame);
	pPerCameraFrameBuffer_->VSSetting(0);
}

void ConstantManager::UpdatePerMergeFrame(CBPerMergeFrame* pCBPerMergeFrame)
{
	pPerMergeFrameBuffer_->Update(pCBPerMergeFrame);
	pPerMergeFrameBuffer_->VSSetting(0);
	pPerMergeFrameBuffer_->PSSetting(0);
}


void ConstantManager::UpdatePerObejct(CBPerObject* cbPerObject)
{
	pPerObjectBuffer_->Update(cbPerObject);
	pPerObjectBuffer_->VSSetting(1);
}


void ConstantManager::CleanUp()
{
	if (nullptr != pPerCameraFrameBuffer_)
	{
		pPerCameraFrameBuffer_->Release();
		pPerCameraFrameBuffer_ = nullptr;
	}

	if (nullptr != pPerMergeFrameBuffer_)
	{
		pPerMergeFrameBuffer_->Release();
		pPerMergeFrameBuffer_ = nullptr;
	}

	if (nullptr != pPerObjectBuffer_)
	{
		pPerObjectBuffer_->Release();
		pPerObjectBuffer_ = nullptr;
	}
}
