#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget(IRenderTarget* pRenderTargetImpl)
	:pRenderTargetImpl_(pRenderTargetImpl)
{
}

RenderTarget::~RenderTarget()
{
	CleanUp();
}

void RenderTarget::Clear()
{
	pRenderTargetImpl_->Clear();
}

void RenderTarget::Setting()
{
	pRenderTargetImpl_->Setting();
}

void RenderTarget::BindRenderTextureForPS(uint32_t slot)
{
	pRenderTargetImpl_->BindRenderTextureForPS(slot);
}

void RenderTarget::ClearRenderTextureForPS(uint32_t slot)
{
	pRenderTargetImpl_->ClearRenderTextureForPS(slot);
}

void RenderTarget::SetClearColor(const Color& cleaColor)
{
	pRenderTargetImpl_->SetClearColor(cleaColor);
}

void RenderTarget::CleanUp()
{
	if (nullptr != pRenderTargetImpl_)
	{
		pRenderTargetImpl_->Release();
		pRenderTargetImpl_ = nullptr;
	}
}
