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
