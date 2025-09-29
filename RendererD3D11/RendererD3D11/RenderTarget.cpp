#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"

D3D11RenderTarget* GCurrentSetRenderTarget = nullptr;

D3D11RenderTarget::D3D11RenderTarget()
	: refCount_(1),
	pRenderTexture_(nullptr),
	pDepthTexture_(nullptr),
	viewport_(),
	clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f })
{
}

D3D11RenderTarget::~D3D11RenderTarget()
{
	CleanUp();
}

HRESULT __stdcall D3D11RenderTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11RenderTarget::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11RenderTarget::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void D3D11RenderTarget::SetClearColor(const Color& color)
{
	clearColor_ = color;
}

bool D3D11RenderTarget::SetTexture(D3D11Texture* pRenderTexture, D3D11Texture* pDepthTexture)
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

//bool D3D11RenderTarget::CreateDepthTexture()
//{
//	if (nullptr == pRenderTexture_)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	pDepthTexture_ = D3D11Texture::Create(pRenderTexture_->Size(), DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL);
//
//	return nullptr != pDepthTexture_;
//}

//bool D3D11RenderTarget::SetRenderTexture(D3D11Texture* pRenderTexture)
//{
//	CleanUp();
//
//	if (nullptr == pRenderTexture)
//	{
//		return false;
//	}
//
//	pRenderTexture_ = pRenderTexture;
//
//	Float2 textureSize = pRenderTexture_->Size();
//	viewport_.TopLeftX = 0.0f;
//	viewport_.TopLeftY = 0.0f;
//	viewport_.Width = textureSize.X;
//	viewport_.Height = textureSize.Y;
//	viewport_.MinDepth = 0.0f;
//	viewport_.MaxDepth = 1.0f;
//
//	return true;
//}

//bool D3D11RenderTarget::SetDepthTexture(D3D11Texture* pDepthTexture)
//{
//	return false;
//}

void D3D11RenderTarget::Clear()
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

void D3D11RenderTarget::Setting()
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

void D3D11RenderTarget::CleanUp()
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
