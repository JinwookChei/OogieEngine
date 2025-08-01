#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: refCount_(1),
	texture_(nullptr),
	renderTargetView_(nullptr),
	depthStencilView_(nullptr),
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

Texture* Texture::Create(const D3D11_TEXTURE2D_DESC& desc)
{
	ID3D11Texture2D* texture;

	HRESULT hr = GRenderer->Device()->CreateTexture2D(&desc, nullptr, &texture);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return nullptr;
	}

	Texture* newTexture = new Texture;
	bool ret = newTexture->SetTexture(texture);
	return newTexture;
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

	texture_ = texture;
	if (nullptr == texture_)
	{
		return true;
	}

	texture_->GetDesc(&desc_);

	if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & desc_.BindFlags)
	{
		bool ret = CreateRenderTargetView();
		if (false == ret)
		{
			return false;
		}
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & desc_.BindFlags)
	{
		bool ret = CreateDepthStencilView();
		if (false == ret)
		{
			return false;
		}
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

bool Texture::CreateDepthStencilView()
{
	if (nullptr == texture_)
	{
		return false;
	}

	if (nullptr != depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}
	HRESULT hr = GRenderer->Device()->CreateDepthStencilView(texture_, nullptr, &depthStencilView_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

ID3D11RenderTargetView* Texture::RenderTargetView() const
{
	return renderTargetView_;
}

ID3D11DepthStencilView* Texture::DepthStencilView() const
{
	return depthStencilView_;
}

void Texture::CleanUp()
{
	if (nullptr != renderTargetView_)
	{
		renderTargetView_->Release();
		renderTargetView_ = nullptr;
	}
	if (nullptr != depthStencilView_)
	{
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}
	if (nullptr != texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
}