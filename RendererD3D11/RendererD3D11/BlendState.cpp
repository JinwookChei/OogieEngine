#include "stdafx.h"
#include "BlendState.h"


BlendState::BlendState()
	: refCount_(1)
	, pCurBlendMode_(nullptr)
	, pBlendOpaque_(nullptr)
	, pBlendAlpha_(nullptr)
	, pBlendAdditive_(nullptr)
	, blendFactor_()
{
}

BlendState::~BlendState()
{
	CleanUp();
}

bool BlendState::Init(ID3D11BlendState* pBlendOpaque, ID3D11BlendState* pBlendAlpha, ID3D11BlendState* pBlendAdditive, float blendFactor[4])
{
	if (nullptr == pBlendOpaque)
	{
		Assert("BlendOpaque is NULL");
		return false;
	}
	if (nullptr == pBlendAlpha)
	{
		Assert("BlendAlpha is NULL");
		return false;
	}
	if (nullptr == pBlendAdditive)
	{
		Assert("BlendAdditive is NULL");
		return false;
	}

	pBlendOpaque_ = pBlendOpaque;
	pBlendAlpha_ = pBlendAlpha;
	pBlendAdditive_ = pBlendAdditive;

	ChangeBlendMode(E_BLEND_MODE_TYPE::OPAQUE_BLEND);
	blendFactor_ = blendFactor_;

	return true;
}

HRESULT __stdcall BlendState::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall BlendState::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall BlendState::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall BlendState::Clear()
{
	float blendFactor[4] = { 0.f, 0.f, 0.f, 0.f };
	GRenderer->DeviceContext()->OMSetBlendState(nullptr, blendFactor, 0xffffffff);
}

void __stdcall BlendState::Setting()
{
	GRenderer->DeviceContext()->OMSetBlendState(pCurBlendMode_, blendFactor_, 0xffffffff);
}

void __stdcall BlendState::ChangeBlendMode(const E_BLEND_MODE_TYPE& blendType)
{
	if (nullptr != pCurBlendMode_)
	{
		pCurBlendMode_->Release();
		pCurBlendMode_ = nullptr;
	}

	switch (blendType)
	{
	case E_BLEND_MODE_TYPE::OPAQUE_BLEND:
		pCurBlendMode_ = pBlendOpaque_;
		pCurBlendMode_->AddRef();
		break;
	case E_BLEND_MODE_TYPE::ALPHA_BLEND:
		pCurBlendMode_ = pBlendAlpha_;
		pCurBlendMode_->AddRef();
		break;
	case E_BLEND_MODE_TYPE::ADDITIVE_BLEND:
		pCurBlendMode_ = pBlendAdditive_;
		pCurBlendMode_->AddRef();
		break;
	default:
		break;
	}
}

void BlendState::CleanUp()
{
	if (nullptr != pCurBlendMode_)
	{
		pCurBlendMode_->Release();
		pCurBlendMode_ = nullptr;
	}
	if (nullptr != pBlendOpaque_)
	{
		pBlendOpaque_->Release();
		pBlendOpaque_ = nullptr;
	}
	if (nullptr != pBlendAlpha_)
	{
		pBlendAlpha_->Release();
		pBlendAlpha_ = nullptr;
	}
	if (nullptr != pBlendAdditive_)
	{
		pBlendAdditive_->Release();
		pBlendAdditive_ = nullptr;
	}
}










BlendStateT::BlendStateT()
	:refCount_(1)
	, blendFactor_{ 0.0f, 0.0f, 0.0f, 0.0f }
	, pBlendState_(nullptr)
{
}

BlendStateT::~BlendStateT()
{
	CleanUp();
}

bool BlendStateT::Init(const E_BLEND_MODE& blendMode)
{
	ID3D11BlendState* pBlendState = nullptr;
	D3D11_BLEND_DESC blendDesc = { 0 };
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	switch (blendMode)
	{
	case E_BLEND_MODE::OPAQUE_BLEND:
	{
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}break;

	case E_BLEND_MODE::ALPHA_BLEND:
	{
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		//blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}break;

	case E_BLEND_MODE::ADDITIVE_BLEND:
	{
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}break;
	default:
		break;
	}

	HRESULT hr = GRenderer->Device()->CreateBlendState(&blendDesc, &pBlendState_);
	if (FAILED(hr))
	{
		Assert("Create BlendState is Fail!!");
		return false;
	}
	
	return true;
}

HRESULT __stdcall BlendStateT::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall BlendStateT::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall BlendStateT::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

BlendStateT* BlendStateT::Create(const E_BLEND_MODE& blendMode)
{
	BlendStateT* pNewBlendState = new BlendStateT;
	if (false == pNewBlendState->Init(blendMode))
	{
		pNewBlendState->Release();
		pNewBlendState = nullptr;
	}

	return pNewBlendState;
}

void BlendStateT::Bind()
{
	UINT samplerMask = 0xffffffff;
	GRenderer->DeviceContext()->OMSetBlendState(pBlendState_, blendFactor_, samplerMask);
}

void BlendStateT::UnBind()
{
	UINT samplerMask = 0xffffffff;
	GRenderer->DeviceContext()->OMSetBlendState(nullptr, blendFactor_, samplerMask);
}

void BlendStateT::CleanUp()
{
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
}