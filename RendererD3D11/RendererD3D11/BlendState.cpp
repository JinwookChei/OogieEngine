#include "stdafx.h"
#include "BlendState.h"

BlendState* BlendState::GOpaqueBlend = nullptr;
BlendState* BlendState::GAlphaBlend = nullptr;
BlendState* BlendState::GAdditiveBlend = nullptr;

BlendState::BlendState()
	:refCount_(1)
	, blendFactor_{ 0.0f, 0.0f, 0.0f, 0.0f }
	, pBlendState_(nullptr)
{
}

BlendState::~BlendState()
{
	CleanUp();
}

bool BlendState::Init(const E_BLEND_PRESET& blendPreset)
{
	D3D11_BLEND_DESC blendDesc = { };
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	D3D11_RENDER_TARGET_BLEND_DESC& rt0 = blendDesc.RenderTarget[0];
	rt0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	switch (blendPreset)
	{
	case E_BLEND_PRESET::OPAQUE_BLEND:
	{
		rt0.BlendEnable = FALSE;
		// 어차피 의미없음.
		//rt0.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		//rt0.SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		//rt0.DestBlend = D3D11_BLEND::D3D11_BLEND_ZERO;
		//rt0.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		//rt0.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		//rt0.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		//rt0.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}break;
	case E_BLEND_PRESET::ALPHA_BLEND:
	{
		rt0.BlendEnable = TRUE;
		rt0.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		rt0.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rt0.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rt0.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rt0.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		rt0.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	}break;

	case E_BLEND_PRESET::ADDITIVE_BLEND:
	{
		rt0.BlendEnable = TRUE;
		rt0.SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		rt0.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		rt0.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		rt0.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rt0.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		rt0.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
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

void BlendState::Bind()
{
	UINT samplerMask = 0xffffffff;
	GRenderer->DeviceContext()->OMSetBlendState(pBlendState_, blendFactor_, samplerMask);
}

void BlendState::UnBind()
{
	UINT samplerMask = 0xffffffff;
	GRenderer->DeviceContext()->OMSetBlendState(nullptr, blendFactor_, samplerMask);
}

void BlendState::CleanUp()
{
	if (nullptr != pBlendState_)
	{
		pBlendState_->Release();
		pBlendState_ = nullptr;
	}
}

void BlendState::InitGlobalBlendStates()
{
	GOpaqueBlend = Create(E_BLEND_PRESET::OPAQUE_BLEND);
	GAlphaBlend = Create(E_BLEND_PRESET::ALPHA_BLEND);
	GAdditiveBlend = Create(E_BLEND_PRESET::ADDITIVE_BLEND);
}

void BlendState::ShutDown()
{
	if(nullptr != BlendState::GOpaqueBlend)
	{
		BlendState::GOpaqueBlend->Release();
		BlendState::GOpaqueBlend = nullptr;
	}
	if (nullptr != BlendState::GAlphaBlend)
	{
		BlendState::GAlphaBlend->Release();
		BlendState::GAlphaBlend = nullptr;
	}
	if (nullptr != BlendState::GAdditiveBlend)
	{
		BlendState::GAdditiveBlend->Release();
		BlendState::GAdditiveBlend = nullptr;
	}
}

BlendState* BlendState::Create(const E_BLEND_PRESET& blendPreset)
{
	BlendState* pNewBlendState = new BlendState;
	if (false == pNewBlendState->Init(blendPreset))
	{
		DEBUG_BREAK();
		pNewBlendState->Release();
		pNewBlendState = nullptr;
	}

	return pNewBlendState;
}