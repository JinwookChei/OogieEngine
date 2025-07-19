#include "stdafx.h"
#include "Texture.h"
#include "RenderTarget.h"


RenderTarget* GCurrentSetRenderTarget = nullptr;

RenderTarget::RenderTarget()
	: refCount_(1),
	texture_(nullptr),
	viewport_()
{
}

RenderTarget::~RenderTarget()
{
	CleanUp();
}

HRESULT __stdcall RenderTarget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall RenderTarget::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall RenderTarget::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool RenderTarget::SetTexture(Texture* texture)
{
	CleanUp();

	if (nullptr == texture)
	{
		DEBUG_BREAK();
		return false;
	}

	texture_ = texture;

	viewport_.TopLeftX = 0.0f;
	viewport_.TopLeftY = 0.0f;
	viewport_.Width = texture_->Width();
	viewport_.Height = texture->Height();
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	return true;
}

void RenderTarget::Clear()
{
	ID3D11RenderTargetView* renderTargetView = texture_->RenderTargetView();

	if (nullptr == renderTargetView)
	{
		DEBUG_BREAK();
		return;
	}

	float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	GRenderer->DeviceContext()->ClearRenderTargetView(renderTargetView, clearColor);
}

void RenderTarget::Setting()
{
	if (GCurrentSetRenderTarget == this)
	{
		return;
	}

	ID3D11RenderTargetView* renderTargetView = texture_->RenderTargetView();
	if (nullptr == renderTargetView)
	{
		DEBUG_BREAK();
		return;
	}

	GCurrentSetRenderTarget = this;

	GRenderer->DeviceContext()->OMSetRenderTargets(1, &renderTargetView, nullptr);
	GRenderer->DeviceContext()->RSSetViewports(1, &viewport_);
}

void RenderTarget::CleanUp()
{
	if (nullptr != texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
}
