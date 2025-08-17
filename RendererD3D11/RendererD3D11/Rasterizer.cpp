#include "stdafx.h"
#include "Rasterizer.h"

Rasterizer::Rasterizer()
	: refCount_(1),
	currentState_(nullptr),
	solidState_(nullptr),
	wireframeState_(nullptr)
{
}

Rasterizer::~Rasterizer()
{
	Cleanup();
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
	GRenderer->DeviceContext()->RSSetState(currentState_);
}

void __stdcall Rasterizer::SetFillMode(FillModeType fillmode)
{
	switch (fillmode)
	{
	case FillModeType::WireFrame:
		currentState_ = wireframeState_;
		break;
	case FillModeType::Solid:
		currentState_ = solidState_;
		break;
	default:
		break;
	}
}

bool Rasterizer::CreateRasterizer(bool frontCounterClockwise, bool backFace)
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = backFace ? D3D11_CULL_MODE::D3D11_CULL_BACK : D3D11_CULL_MODE::D3D11_CULL_FRONT;
	desc.FrontCounterClockwise = frontCounterClockwise ? TRUE : FALSE;

	desc.FillMode = D3D11_FILL_WIREFRAME;
	HRESULT hr = GRenderer->Device()->CreateRasterizerState(&desc, &wireframeState_);
	if (FAILED(hr))
	{
		return false;
	}

	desc.FillMode = D3D11_FILL_SOLID;
	hr = GRenderer->Device()->CreateRasterizerState(&desc, &solidState_);
	if (FAILED(hr))
	{
		return false;
	}

	SetFillMode(FillModeType::Solid);

	return true;
}

void Rasterizer::Cleanup()
{
	currentState_ = nullptr;

	if (nullptr != solidState_)
	{
		solidState_->Release();
		solidState_ = nullptr;
	}
	if (nullptr != wireframeState_)
	{
		wireframeState_->Release();
		wireframeState_ = nullptr;
	}
}
