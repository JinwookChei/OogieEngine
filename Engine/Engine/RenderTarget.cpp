#include "stdafx.h"
#include "RenderTarget.h"

//RenderTarget::RenderTarget()
//	: pRenderTargetImpl_(nullptr)
//{
//}

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
