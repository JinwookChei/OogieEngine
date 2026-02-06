#include "stdafx.h"
#include "DepthState.h"

DepthState* DepthState::GDepthEnableWrite = nullptr;
DepthState* DepthState::GDepthEnableReadOnly = nullptr;
DepthState* DepthState::GDepthDisable = nullptr;

DepthState::DepthState()
	: refCount_(1)
	, enableDepth_(false)
	, writeDepth_(false)
	, pDepthState_(nullptr)
{
}

DepthState::~DepthState()
{
	CleanUp();
}

HRESULT __stdcall DepthState::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall DepthState::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall DepthState::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}


bool DepthState::Init(bool enableDepthTest, bool wirteDepth)
{
	enableDepth_ = enableDepthTest;
	writeDepth_ = wirteDepth;

	D3D11_DEPTH_STENCIL_DESC pDepthStateDesc = {};
	pDepthStateDesc.DepthEnable = enableDepthTest ? TRUE : FALSE;
	pDepthStateDesc.DepthFunc = enableDepthTest ? D3D11_COMPARISON_LESS_EQUAL : D3D11_COMPARISON_ALWAYS;
	pDepthStateDesc.DepthWriteMask = wirteDepth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	pDepthStateDesc.StencilEnable = FALSE;

	HRESULT hr = GRenderer->Device()->CreateDepthStencilState(&pDepthStateDesc, &pDepthState_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void DepthState::InitGlobalDepthStates()
{
	DepthState::GDepthEnableWrite = Create(true, true);
	DepthState::GDepthEnableReadOnly = Create(true, false);
	DepthState::GDepthDisable = Create(false, false);
}

DepthState* DepthState::Create(bool enableDepthTest, bool wirteDepth)
{
	DepthState* pNewDepthState = new DepthState;
	if (false == pNewDepthState->Init(enableDepthTest, wirteDepth))
	{
		DEBUG_BREAK();
		pNewDepthState->Release();
		pNewDepthState = nullptr;
	}

	return pNewDepthState;
}


void DepthState::Bind()
{
	GRenderer->DeviceContext()->OMSetDepthStencilState(pDepthState_, 0);
}

void DepthState::UnBind()
{
	ID3D11DepthStencilState* pNullState = nullptr;
	GRenderer->DeviceContext()->OMSetDepthStencilState(pNullState, 0);
}

void DepthState::CleanUp()
{
	if (nullptr != pDepthState_)
	{
		pDepthState_->Release();
		pDepthState_ = nullptr;
	}
}