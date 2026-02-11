#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"

IRenderTarget* GCurrentSetRenderTarget = nullptr;

RenderTarget::RenderTarget()
	: refCount_(1)
	, viewport_()
	, clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f })
	, desc_()
	, pRenderTexture_(nullptr)
	, pDepthTexture_(nullptr)
	, pRTV_(nullptr)
	, pSRVs_()
	, pDSV_(nullptr)
{
}

RenderTarget::~RenderTarget()
{
	CleanUp();
}

bool RenderTarget::Init(const RenderTargetDesc& desc, Texture* pRenderTexture, Texture* pDepthTexture)
{
	SetClearColor(desc.clearColor_);
	desc_ = desc.forwardDesc_;
	
	bool ret = SetTexture(pRenderTexture, pDepthTexture);
	if (false == ret)
	{
		DEBUG_BREAK();
		return false;
	}

	if (!desc_.useDepthStencil_)
	{
		pRTV_ = pRenderTexture->RenderTargetView();
		pDSV_ = nullptr;
		pSRVs_[0] = pRenderTexture->ShaderResourceView();
		pSRVs_[1] = nullptr;
	}
	else
	{
		pRTV_ = pRenderTexture->RenderTargetView();
		pDSV_ = pDepthTexture_->DepthStencilView();
		pSRVs_[0] = pRenderTexture->ShaderResourceView();
		pSRVs_[1] = pDepthTexture_->ShaderResourceView();
	}

	
	return true;
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


void __stdcall RenderTarget::Bind()
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

void __stdcall RenderTarget::Bind(ITexture* pDepthTexture)
{
	if (nullptr == pDepthTexture)
	{
		DEBUG_BREAK();
		return;
	}

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


	Texture* pDepthTexturea = static_cast<Texture*>(pDepthTexture);
	ID3D11DepthStencilView* pDepthStencilView = pDepthTexturea->DepthStencilView();
	if (nullptr == pDepthStencilView)
	{
		DEBUG_BREAK();
		return;
	}

	GCurrentSetRenderTarget = this;
	GRenderer->DeviceContext()->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);
	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
}

void __stdcall RenderTarget::UnBind()
{
	GCurrentSetRenderTarget = nullptr;
	ID3D11RenderTargetView* pRTV = nullptr;
	GRenderer->DeviceContext()->OMSetRenderTargets(1, &pRTV, nullptr);
}

void RenderTarget::Clear()
{
	ID3D11RenderTargetView* pRenderTargetView = pRenderTexture_->RenderTargetView();

	if (nullptr == pRenderTargetView)
	{
		DEBUG_BREAK(); 
	}

	GRenderer->DeviceContext()->ClearRenderTargetView(pRenderTargetView, clearColor_.Arr1D);

	if (!desc_.useDepthStencil_)
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



RenderTargetDesc __stdcall RenderTarget::GetDesc() const
{
	RenderTargetDesc desc{ E_RENDER_TECHNIQUE_TYPE::Forward };
	desc.clearColor_ = clearColor_;
	desc.forwardDesc_ = desc_;
	desc.size_ = pRenderTexture_->Size();

	return desc;
}

Float2 __stdcall RenderTarget::GetSize() const
{
	return pRenderTexture_->Size();
}

void __stdcall RenderTarget::SetClearColor(const Color& color)
{
	clearColor_ = color;
}


void __stdcall RenderTarget::BindRenderTexturePS(uint32_t slot)
{
	if(!desc_.useDepthStencil_)
	{
		pRenderTexture_->BindRenderTextureForPS(slot);
	}
	else
	{
		GRenderer->DeviceContext()->PSSetShaderResources(slot, 2, pSRVs_);
	}
}

void __stdcall RenderTarget::UnBindRenderTexturePS(uint32_t slot)
{
	if (!desc_.useDepthStencil_)
	{
		pRenderTexture_->ClearRenderTextureForPS(slot);
	}
	else
	{
		ID3D11ShaderResourceView* pNullSrvs[2] = { nullptr, nullptr};
		GRenderer->DeviceContext()->PSSetShaderResources(slot, 2, pNullSrvs);
	}
}

//void __stdcall RenderTarget::EndRenderPass()
//{
//	GCurrentSetRenderTarget = nullptr;
//
//	ID3D11RenderTargetView* pRTV = nullptr;
//	GRenderer->DeviceContext()->OMSetRenderTargets(1, &pRTV, nullptr);
//}

void* __stdcall RenderTarget::GetShaderResourceView(const E_RENDER_TEXTURE_TYPE& texureType)
{
	return pSRVs_[0];
}

ITexture* __stdcall RenderTarget::GetRenderTexture(const E_RENDER_TEXTURE_TYPE& textureType)
{
	switch (textureType)
	{
	case E_RENDER_TEXTURE_TYPE::Albedo:
		return pRenderTexture_;
		break;
	case E_RENDER_TEXTURE_TYPE::Normal:
		DEBUG_BREAK();
		break;
	case E_RENDER_TEXTURE_TYPE::Specular:
		DEBUG_BREAK();
		break;
	case E_RENDER_TEXTURE_TYPE::Depth:
		return pDepthTexture_;
		break;
	default:
		break;
	}

	return nullptr;
}

//ITexture* __stdcall RenderTarget::GetDepthTexture()
//{
//	return pDepthTexture_;
//}


bool RenderTarget::SetTexture(Texture* pRenderTexture, Texture* pDepthTexture)
{
	CleanUp();

	if (nullptr == pRenderTexture)
	{
		return false;
	}

	pRenderTexture_ = pRenderTexture;

	if(desc_.useDepthStencil_)
	{
		if (nullptr == pDepthTexture)
		{
			DEBUG_BREAK();
			return false;
		}
		pDepthTexture_ = pDepthTexture;
	}


	Float2 textureSize = pRenderTexture_->Size();
	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.Width = textureSize.X;
	viewport_.Height = textureSize.Y;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	return true;
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
