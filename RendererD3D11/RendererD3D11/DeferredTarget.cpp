#include "stdafx.h"
//#include "Texture.h"
//#include "DeferredTarget.h"
//
//DeferredTarget::DeferredTarget()
//	: refCount_(1),
//	viewport_(),
//	desc_(),
//	pRenderTextureAlbedo_(nullptr),
//	pRenderTextureNormal_(nullptr),
//	pRenderTextureMaterial_(nullptr),
//	pDepthTexture_(nullptr),
//	pRtvs_{nullptr, nullptr, nullptr}
//{
//}
//
//DeferredTarget::~DeferredTarget()
//{
//	CleanUp();
//}
//
//bool DeferredTarget::Init()
//{
//	return true;
//}
//
//HRESULT __stdcall DeferredTarget::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall DeferredTarget::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall DeferredTarget::Release()
//{
//	--refCount_;
//	ULONG tmpRefCount = refCount_;
//	if (0 == refCount_) {
//		delete this;
//	}
//
//	return tmpRefCount;
//}
//
//void __stdcall DeferredTarget::Clear()
//{
//	ClearRenderTexture(pRenderTextureAlbedo_);
//	ClearRenderTexture(pRenderTextureNormal_);
//	ClearRenderTexture(pRenderTextureMaterial_);
//
//	ClearDepthTexture(pDepthTexture_);
//}
//
//void __stdcall DeferredTarget::Setting()
//{
//	if (GCurrentSetRenderTarget == this)
//	{
//		return;
//	}
//
//	GCurrentSetRenderTarget = this;
//	ID3D11DepthStencilView* pDsv = pDepthTexture_->DepthStencilView();
//	if (nullptr == pDsv)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	GRenderer->DeviceContext()->OMSetRenderTargets(rtvCnt, pRtvs_, pDsv);
//	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
//}
//
//void __stdcall DeferredTarget::BindRenderTextureForPS(uint32_t slot)
//{
//	ID3D11ShaderResourceView* pSrvs[4] = 
//	{
//		pRenderTextureAlbedo_->ShaderResourceView(), 
//		pRenderTextureNormal_->ShaderResourceView(), 
//		pRenderTextureMaterial_->ShaderResourceView(),
//		pDepthTexture_->ShaderResourceView()
//	};
//
//	GRenderer->DeviceContext()->PSSetShaderResources(slot, 4, pSrvs);
//}
//
//void __stdcall DeferredTarget::ClearRenderTextureForPS(uint32_t slot)
//{
//	ID3D11ShaderResourceView* pNullSrvs[4] = { nullptr, nullptr, nullptr, nullptr };
//	GRenderer->DeviceContext()->PSSetShaderResources(slot, 4, pNullSrvs);
//}
//
//void __stdcall DeferredTarget::SetClearColor(const Color& color)
//{
//}
//
//void DeferredTarget::ClearRenderTexture(Texture* pRenderTexture)
//{
//	ID3D11RenderTargetView* pRenderTargetView = pRenderTexture->RenderTargetView();
//	if(nullptr == pRenderTargetView)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	//GRenderer->DeviceContext()->ClearRenderTargetView(pRenderTargetView, desc_.clearColor.Arr1D);
//
//}
//
//void DeferredTarget::ClearDepthTexture(Texture* pDepthTexture)
//{
//	ID3D11DepthStencilView* pDepthStencilView = pDepthTexture->DepthStencilView();
//	if (nullptr == pDepthStencilView)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	GRenderer->DeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
//}
