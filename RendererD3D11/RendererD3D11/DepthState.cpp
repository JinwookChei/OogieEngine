#include "stdafx.h"
#include "DepthState.h"


DepthState::DepthState()
	: refCount_(1)
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


bool DepthState::Init(bool useDepthTest, bool wirteDepth)
{
	D3D11_DEPTH_STENCIL_DESC pDepthStateDesc = {};
	pDepthStateDesc.DepthEnable = useDepthTest ? TRUE : FALSE;
	pDepthStateDesc.DepthWriteMask = wirteDepth ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
	pDepthStateDesc.DepthFunc = useDepthTest ? D3D11_COMPARISON_LESS_EQUAL : D3D11_COMPARISON_ALWAYS;
	pDepthStateDesc.StencilEnable = FALSE;

	HRESULT hr = GRenderer->Device()->CreateDepthStencilState(&pDepthStateDesc, &pDepthState_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

DepthState* DepthState::Create(bool useDepthTest, bool wirteDepth)
{
	DepthState* pNewDepthState = new DepthState;
	if (false == pNewDepthState->Init(useDepthTest, wirteDepth))
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