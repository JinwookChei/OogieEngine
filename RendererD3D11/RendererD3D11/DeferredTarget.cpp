#include "stdafx.h"
#include "Texture.h"
#include "DeferredTarget.h"

DeferredTarget::DeferredTarget()
	: refCount_(1),
	viewport_(),
	clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f }),
	desc_(),
	pRenderTextureAlbedo_(nullptr),
	pRenderTextureNormal_(nullptr),
	pRenderTextureMaterial_(nullptr),
	pDepthTexture_(nullptr),
	//pRTVs_{ nullptr, nullptr, nullptr },
	//pSRVs_{nullptr,nullptr, nullptr, nullptr},
	pDSV_(nullptr)
{
	std::fill(pRTVs_, pRTVs_ + RENDER_BUFFER_COUNT, nullptr);
	std::fill(pSRVs_, pSRVs_ + RESOURCE_BUFFER_COUNT, nullptr);
}

DeferredTarget::~DeferredTarget()
{
	CleanUp();
}

bool DeferredTarget::Init
(
	const RenderTargetDesc& desc,
	Texture* pRenderTextureAlbedo, 
	Texture* pRenderTextureNormal, 
	Texture* pRenderTextureMaterial, 
	Texture* pDepthTexture)
{
	SetClearColor(desc.clearColor_);
	desc_ = desc.deferredDesc_;

	bool ret = SetTexture(pRenderTextureAlbedo, pRenderTextureNormal, pRenderTextureMaterial, pDepthTexture);
	if (false == ret)
	{
		Assert("SetTexture() == false");
		return false;
	}

	pRTVs_[0] = pRenderTextureAlbedo->RenderTargetView();
	pRTVs_[1] = pRenderTextureNormal->RenderTargetView();
	pRTVs_[2] = pRenderTextureMaterial->RenderTargetView();

	for (int i = 0; i < RENDER_BUFFER_COUNT; ++i)
	{
		if (nullptr == pRTVs_[i])
		{
			Assert("RenderTargetView is NULL");
			return false;
		}
	}

	pSRVs_[0] = pRenderTextureAlbedo->ShaderResourceView();
	pSRVs_[1] = pRenderTextureNormal->ShaderResourceView();
	pSRVs_[2] = pRenderTextureMaterial->ShaderResourceView();
	pSRVs_[3] = pDepthTexture_->ShaderResourceView();

	for (int i = 0; i < RESOURCE_BUFFER_COUNT; ++i)
	{
		if (nullptr == pSRVs_[i])
		{
			Assert("ShaderResouceView is NULL");
			return false;
		}
	}

	pDSV_ = pDepthTexture_->DepthStencilView();
	if (nullptr == pDSV_)
	{
		Assert("DepthStencilView is NULL");
		return false;
	}

	return true;
}


HRESULT __stdcall DeferredTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall DeferredTarget::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall DeferredTarget::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall DeferredTarget::Clear()
{
	ClearRenderTexture(pRenderTextureAlbedo_);
	ClearRenderTexture(pRenderTextureNormal_);
	ClearRenderTexture(pRenderTextureMaterial_);

	ClearDepthTexture(pDepthTexture_);
}

void __stdcall DeferredTarget::Setting()
{
	if (GCurrentSetRenderTarget == this)
	{
		return;
	}

	GCurrentSetRenderTarget = this;

	GRenderer->DeviceContext()->OMSetRenderTargets(RENDER_BUFFER_COUNT, pRTVs_, pDSV_);
	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
}

RenderTargetDesc __stdcall DeferredTarget::GetDesc() const
{
	RenderTargetDesc desc{ ERenderTechniqueType::Deferred };
	desc.clearColor_ = clearColor_;
	desc.deferredDesc_ = desc_;
	desc.size_ = pRenderTextureAlbedo_->Size();

	return desc;
}

Float2 __stdcall DeferredTarget::GetSize() const
{
	return pRenderTextureAlbedo_->Size();
}

void __stdcall DeferredTarget::SetClearColor(const Color& color)
{
	clearColor_ = color;
}

void __stdcall DeferredTarget::BindRenderTextureForPS(uint32_t slot)
{
	GRenderer->DeviceContext()->PSSetShaderResources(slot, RESOURCE_BUFFER_COUNT, pSRVs_);
}

void __stdcall DeferredTarget::ClearRenderTextureForPS(uint32_t slot)
{
	ID3D11ShaderResourceView* pNullSrvs[RESOURCE_BUFFER_COUNT] = { nullptr, nullptr, nullptr, nullptr };
	GRenderer->DeviceContext()->PSSetShaderResources(slot, RESOURCE_BUFFER_COUNT, pNullSrvs);
}

void __stdcall DeferredTarget::EndRenderPass()
{
	ID3D11RenderTargetView* nullRTV[RENDER_BUFFER_COUNT] = { nullptr, nullptr, nullptr };

	GRenderer->DeviceContext()->OMSetRenderTargets(RENDER_BUFFER_COUNT, nullRTV, nullptr);
}

bool DeferredTarget::SetTexture(Texture* pRenderTextureAlbedo, Texture* pRenderTextureNormal, Texture* pRenderTextureMaterial, Texture* pDepthTexture)
{
	CleanUp();

	if (nullptr == pRenderTextureAlbedo || nullptr == pRenderTextureNormal || nullptr == pRenderTextureMaterial || nullptr == pDepthTexture)
	{
		Assert("SetTexture() = False");
		return false;
	}

	pRenderTextureAlbedo_ = pRenderTextureAlbedo;
	pRenderTextureNormal_ = pRenderTextureNormal;
	pRenderTextureMaterial_ = pRenderTextureMaterial;
	pDepthTexture_ = pDepthTexture;

	Float2 textureSize = pRenderTextureAlbedo_->Size();
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

void DeferredTarget::ClearRenderTexture(Texture* pRenderTexture)
{
	ID3D11RenderTargetView* pRenderTargetView = pRenderTexture->RenderTargetView();
	if (nullptr == pRenderTargetView)
	{
		Assert("ClearRenderTexture() FAIL");
		return;
	}

	GRenderer->DeviceContext()->ClearRenderTargetView(pRenderTargetView, clearColor_.Arr1D);
}

void DeferredTarget::ClearDepthTexture(Texture* pDepthTexture)
{
	ID3D11DepthStencilView* pDepthStencilView = pDepthTexture->DepthStencilView();
	if (nullptr == pDepthStencilView)
	{
		Assert("ClearDepthTexture() FAIL");
		return;
	}

	GRenderer->DeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1.0f, 0);
}


void DeferredTarget::CleanUp()
{
	if (nullptr != pRenderTextureAlbedo_)
	{
		pRenderTextureAlbedo_->Release();
		pRenderTextureAlbedo_ = nullptr;
	}
	if (nullptr != pRenderTextureNormal_)
	{
		pRenderTextureNormal_->Release();
		pRenderTextureNormal_ = nullptr;
	}
	if (nullptr != pRenderTextureMaterial_)
	{
		pRenderTextureMaterial_->Release();
		pRenderTextureMaterial_ = nullptr;
	}
	if (nullptr != pDepthTexture_)
	{
		pDepthTexture_->Release();
		pDepthTexture_ = nullptr;
	}
}