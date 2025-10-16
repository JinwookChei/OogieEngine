#include "stdafx.h"
#include "Rasterizer.h"


Rasterizer::Rasterizer(IRasterizer* pRasterizer)
	:pRasterizerImpl_(pRasterizer)
{
}

Rasterizer::~Rasterizer()
{
	CleanUp();
}

void Rasterizer::Setting()
{
	if (nullptr == pRasterizerImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pRasterizerImpl_->Setting();
}

void Rasterizer::SetFillMode(EFillModeType fillModeType)
{
	if (nullptr == pRasterizerImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pRasterizerImpl_->SetFillMode(fillModeType);
}

void Rasterizer::SetRasterizer(IRasterizer* pRasterizer)
{
	if (nullptr != pRasterizerImpl_)
	{
		pRasterizerImpl_->Release();
		pRasterizerImpl_ = nullptr;
	}

	if (nullptr != pRasterizer)
	{
		pRasterizerImpl_ = pRasterizer;
		pRasterizerImpl_->AddRef();
	}
}

void Rasterizer::CleanUp()
{
	if (nullptr != pRasterizerImpl_)
	{
		pRasterizerImpl_->Release();
		pRasterizerImpl_ = nullptr;
	}
}