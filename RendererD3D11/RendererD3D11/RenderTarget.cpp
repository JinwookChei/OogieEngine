#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"

RenderTarget* GCurrentSetRenderTarget = nullptr;

RenderTarget::RenderTarget()
	: refCount_(1),
	pRenderTexture_(nullptr),
	pDepthTexture_(nullptr),
	viewport_(),
	clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f })
{
}

RenderTarget::~RenderTarget()
{
	CleanUp();
}

HRESULT __stdcall RenderTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall RenderTarget::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall RenderTarget::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void RenderTarget::Clear()
{
	ID3D11RenderTargetView* pRenderTargetView = pRenderTexture_->RenderTargetView();

	if (nullptr == pRenderTargetView)
	{
		DEBUG_BREAK();
		return;
	}
	FLOAT ColorRGBA[4] = { 0.1f, 0.2f, 0.4f, 1.0f };
	GRenderer->DeviceContext()->ClearRenderTargetView(pRenderTargetView, clearColor_.Arr1D);

	if (nullptr == pDepthTexture_)
	{
		// DepthStencil View를 사용하지 않는 경우. DepthStencilView를 Clear 하지 않음.
		return;
	}

	ID3D11DepthStencilView* pDepthStencilView = pDepthTexture_->DepthStencilView();
	if (nullptr == pDepthStencilView)
	{
		DEBUG_BREAK();
		return;
	}

	GRenderer->DeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderTarget::Setting()
{
	if (GCurrentSetRenderTarget == this)
	{
		return;
	}

	ID3D11RenderTargetView* pRenderTargetView = pRenderTexture_->RenderTargetView();
	if (nullptr == pRenderTargetView)
	{
		DEBUG_BREAK();
		return;
	}

	ID3D11DepthStencilView* pDepthStencilView = nullptr;
	if (nullptr != pDepthTexture_)
	{
		pDepthStencilView = pDepthTexture_->DepthStencilView();
	}

	GCurrentSetRenderTarget = this;

	GRenderer->DeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
}

void __stdcall RenderTarget::BindRenderTextureForPS(uint32_t slot)
{
	pRenderTexture_->BindRenderTextureForPS(slot);
}

void __stdcall RenderTarget::ClearRenderTextureForPS(uint32_t slot)
{
	pRenderTexture_->ClearRenderTextureForPS(slot);
}


bool RenderTarget::SetTexture(Texture* pRenderTexture, Texture* pDepthTexture)
{
	CleanUp();

	if (nullptr == pRenderTexture)
	{
		return false;
	}

	pRenderTexture_ = pRenderTexture;

	Float2 textureSize = pRenderTexture_->Size();
	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.Width = textureSize.X;
	viewport_.Height = textureSize.Y;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	if (nullptr != pDepthTexture)
	{
		pDepthTexture_ = pDepthTexture;
	}

	return true;
}

void RenderTarget::SetClearColor(const Color& color)
{
	clearColor_ = color;
}

void RenderTarget::CleanUp()
{
	if (nullptr != pRenderTexture_)
	{
		pRenderTexture_->Release();
		pRenderTexture_ = nullptr;
	}

	if (nullptr != pDepthTexture_)
	{
		pDepthTexture_->Release();
		pDepthTexture_ = nullptr;
	}

}
