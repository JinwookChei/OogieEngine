#include "stdafx.h"
#include "Rasterizer.h"

Rasterizer::Rasterizer(ID3D11RasterizerState* pSolidState, ID3D11RasterizerState* pWireframeState)
	: refCount_(1),
	pCurrentState_(nullptr),
	pSolidState_(pSolidState),
	pWireframeState_(pWireframeState)
{
}

Rasterizer::~Rasterizer()
{
	CleanUp();
}

HRESULT __stdcall Rasterizer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Rasterizer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Rasterizer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall Rasterizer::Setting()
{
	GRenderer->DeviceContext()->RSSetState(pCurrentState_);
}

void __stdcall Rasterizer::SetFillMode(EFillModeType fillmode)
{
	if (nullptr != pCurrentState_)
	{
		pCurrentState_->Release();
		pCurrentState_ = nullptr;
	}
	
	switch (fillmode)
	{
	case EFillModeType::WireFrame:
		pCurrentState_ = pWireframeState_;
		pCurrentState_->AddRef();
		break;
	case EFillModeType::Solid:
		pCurrentState_ = pSolidState_;
		pCurrentState_->AddRef();
		break;
	default:
		break;
	}
}

void Rasterizer::CleanUp()
{
	if (nullptr != pCurrentState_)
	{
		pCurrentState_->Release();
		pCurrentState_ = nullptr;
	}

	if (nullptr != pSolidState_)
	{
		pSolidState_->Release();
		pSolidState_ = nullptr;
	}
	if (nullptr != pWireframeState_)
	{
		pWireframeState_->Release();
		pWireframeState_ = nullptr;
	}
}
