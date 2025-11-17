#include "stdafx.h"
#include "RasterizerManager.h"

RasterizerManager::RasterizerManager()
	: pRasterizer_(nullptr)
{
	pRasterizer_ = GRenderer->CreateRasterizer(false, true);
	if (nullptr == pRasterizer_)
	{
		Assert("CreateRasterizer is Fail!!");
		return;
	}
}

RasterizerManager::~RasterizerManager()
{
	CleanUp();
}

RasterizerManager* RasterizerManager::Instance()
{
	return GRasterizerManager;
}

void RasterizerManager::Setting(E_FILLMODE_TYPE fillModeType)
{
	pRasterizer_->ChangeFillMode(fillModeType);
	pRasterizer_->Setting();
}


void RasterizerManager::CleanUp()
{
	if (nullptr != pRasterizer_)
	{
		pRasterizer_->Release();
		pRasterizer_ = nullptr;
	}
}