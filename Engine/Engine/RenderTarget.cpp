#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
	: pRenderTargetImpl_(nullptr)
{
}

RenderTarget::~RenderTarget()
{
	CleanUP();
}

void RenderTarget::CleanUP()
{
	if (nullptr != pRenderTargetImpl_)
	{
		pRenderTargetImpl_->Release();
		pRenderTargetImpl_ = nullptr;
	}
}
