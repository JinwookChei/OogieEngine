#include "stdafx.h"
#include "Rasterizer.h"

Rasterizer* Rasterizer::GRasterizer = nullptr;

Rasterizer::Rasterizer()
	: refCount_(1),
	pCurrentState_(nullptr),
	pSolidState_(nullptr),
	pWireframeState_(nullptr)
{
}

Rasterizer::~Rasterizer()
{
	CleanUp();
}

bool Rasterizer::Init(ID3D11RasterizerState* pSolidState, ID3D11RasterizerState* pWireframeState)
{
	if (nullptr == pSolidState)
	{
		DEBUG_BREAK();
		return false;
	}
	
	if (nullptr == pWireframeState)
	{
		DEBUG_BREAK();
		return false;
	}
	
	pSolidState_ = pSolidState;
	pWireframeState_ = pWireframeState;

	ChangeFillMode(E_FILLMODE_TYPE::SOLID);
	return true;
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

void __stdcall Rasterizer::ChangeFillMode(E_FILLMODE_TYPE fillmode)
{
	if (nullptr != pCurrentState_)
	{
		pCurrentState_->Release();
		pCurrentState_ = nullptr;
	}

	switch (fillmode)
	{
	case E_FILLMODE_TYPE::WIREFRAME:
		pCurrentState_ = pWireframeState_;
		pCurrentState_->AddRef();
		break;
	case E_FILLMODE_TYPE::SOLID:
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

void Rasterizer::InitGlobalRasterizer()
{
	Rasterizer::GRasterizer = static_cast<Rasterizer*>(GResourceFactory->CreateRasterizer(false, true));
}

void Rasterizer::ShutDown()
{
	if (nullptr != Rasterizer::GRasterizer)
	{
		Rasterizer::GRasterizer->Release();
		Rasterizer::GRasterizer = nullptr;
	}
}