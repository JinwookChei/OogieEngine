#include "stdafx.h"
#include "Rasterizer.h"

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

void __stdcall Rasterizer::SetFillMode(FillModeType fillmode)
{
	if (nullptr != pCurrentState_)
	{
		pCurrentState_->Release();
		pCurrentState_ = nullptr;
	}
	
	switch (fillmode)
	{
	case FillModeType::WireFrame:
		pCurrentState_ = pWireframeState_;
		pCurrentState_->AddRef();
		break;
	case FillModeType::Solid:
		pCurrentState_ = pSolidState_;
		pCurrentState_->AddRef();
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
	HRESULT hr = GRenderer->Device()->CreateRasterizerState(&desc, &pWireframeState_);
	if (FAILED(hr))
	{
		return false;
	}

	// 메모리 누수 디버깅용 이름 설정.
	const char* debugObjectName = "Rasterizer::pWireframeState_";
	pWireframeState_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);

	desc.FillMode = D3D11_FILL_SOLID;
	hr = GRenderer->Device()->CreateRasterizerState(&desc, &pSolidState_);
	if (FAILED(hr))
	{
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	debugObjectName = "Rasterizer::pSolidState_";
	pSolidState_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	SetFillMode(FillModeType::Solid);
	return true;
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
