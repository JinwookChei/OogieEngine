#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: refCount_(1),
	texture_(nullptr),
	renderTargetView_(nullptr),
	desc_()
{
}

Texture::~Texture()
{
	CleanUp();
}

HRESULT __stdcall Texture::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Texture::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Texture::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}
	return tmpRefCount;
}

float Texture::Width() const
{
	return (float)desc_.Width;
}

float Texture::Height() const
{
	return (float)desc_.Height;
}

bool Texture::SetTexture(ID3D11Texture2D* texture)
{
	CleanUp();

	if (nullptr == texture)
	{
		return true;
	}

	texture_ = texture;
	texture_->GetDesc(&desc_);

	if (false == CreateRenderTargetView())
	{
		return false;
	}
	
	return true;
}

bool Texture::CreateRenderTargetView()
{
	if (nullptr == texture_)
	{
		return false;
	}

	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateRenderTargetView(texture_, nullptr, &renderTargetView_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

ID3D11RenderTargetView* Texture::RenderTargetView()
{
	return renderTargetView_;
}


void Texture::CleanUp()
{
	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}
	if (nullptr != texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
}