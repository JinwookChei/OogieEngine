#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"


RenderTarget* GCurrentSetRenderTarget = nullptr;

RenderTarget::RenderTarget()
	: refCount_(1),
	renderTexture_(nullptr),
	depthTexture_(nullptr),
	viewport_(),
	clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f })
{
}

RenderTarget::~RenderTarget()
{
	Cleanup();
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

bool RenderTarget::CreateDepthTexture()
{
	D3D11_TEXTURE2D_DESC desc = { 0, };
	desc.Width = renderTexture_->Width();
	desc.Height = renderTexture_->Height();
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	depthTexture_ = Texture::Create(desc);
	return nullptr != depthTexture_;
}

bool RenderTarget::SetTexture(Texture* texture)
{
	Cleanup();

	if (nullptr == texture)
	{
		return false;
	}

	renderTexture_ = texture;

	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.Width = renderTexture_->Width();
	viewport_.Height = renderTexture_->Height();
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	return true;
}

void RenderTarget::Clear()
{
	ID3D11RenderTargetView* renderTargetView = renderTexture_->RenderTargetView();

	if (nullptr == renderTargetView)
	{
		DEBUG_BREAK();
		return;
	}
	
	GRenderer->DeviceContext()->ClearRenderTargetView(renderTargetView, clearColor_.Array1D);

	if (nullptr == depthTexture_)
	{
		// DepthStencil View를 사용하지 않는 경우. DepthStencilView를 Clear 하지 않음.
		return;
	}

	ID3D11DepthStencilView* depthStencilView = depthTexture_->DepthStencilView();
	if (nullptr == depthStencilView)
	{
		DEBUG_BREAK();
		return;
	}

	GRenderer->DeviceContext()->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}	

void RenderTarget::Setting()
{
	if (GCurrentSetRenderTarget == this)
	{
		return;
	}

	ID3D11RenderTargetView* renderTargetView = renderTexture_->RenderTargetView();
	if (nullptr == renderTargetView)
	{
		DEBUG_BREAK();
		return;
	}

	ID3D11DepthStencilView* depthStencilView = depthTexture_->DepthStencilView();
	if (nullptr == depthStencilView)
	{
		DEBUG_BREAK();
		return;
	}

	GCurrentSetRenderTarget = this;
	GRenderer->DeviceContext()->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
}

void RenderTarget::Cleanup()	
{
	if (nullptr != renderTexture_)
	{
		renderTexture_->Release();
		renderTexture_ = nullptr;
	}

	if (nullptr != depthTexture_)
	{
		depthTexture_->Release();
		depthTexture_ = nullptr;
	}

}
