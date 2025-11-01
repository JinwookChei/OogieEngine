#include "stdafx.h"
#include "BlendState.h"


BlendState::BlendState()
	: refCount_(1),
	pBlendState_(nullptr),
	blendFactor_(nullptr)
{
}

BlendState::~BlendState()
{
	CleanUp();
}

bool BlendState::Init(ID3D11BlendState* pBlendState, float blendFactor[4])
{
	if (nullptr == pBlendState)
	{
		Assert("BlendState is NULL");
		return false;
	}

	pBlendState_ = pBlendState;

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
	GRenderer->DeviceContext()->OMSetBlendState(pBlendState_, blendFactor_, 0xffffffff);
}

void BlendState::CleanUp()
{
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
}
