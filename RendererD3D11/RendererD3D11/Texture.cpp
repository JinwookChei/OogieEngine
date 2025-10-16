#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: refCount_(1),
	pTexture_(nullptr),
	pRenderTargetView_(nullptr),
	pDepthStencilView_(nullptr),
	pShaderResourceView_(nullptr),
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

Texture* Texture::Create(const Float2& size, DXGI_FORMAT format, uint32_t flag)
{
	D3D11_TEXTURE2D_DESC desc = { 0, };
	desc.ArraySize = 1;
	desc.Width = (UINT)size.X;
	desc.Height = (UINT)size.Y;
	desc.Format = format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.MipLevels = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = flag;

	return Create(desc);
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
	if (ret == false)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return newTexture;
}


void Texture::BindRenderTextureForPS(uint32_t slot)
{
	if (nullptr == pShaderResourceView_)
	{
		DEBUG_BREAK();
		return;
	}

	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pShaderResourceView_);
}

void Texture::ClearRenderTextureForPS(uint32_t slot)
{
	ID3D11ShaderResourceView* pNull = nullptr;

	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pNull);
}

bool Texture::SetTexture(ID3D11Texture2D* pTexture)
{
	CleanUp();

	pTexture_ = pTexture;
	if (nullptr == pTexture_)
	{
		return true;
	}

	pTexture_->GetDesc(&desc_);

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

	if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & desc_.BindFlags)
	{
		bool ret = CreateShaderResourceView();
		if (false == ret)
		{
			return false;
		}
	}

	return true;
}

Float2 Texture::Size() const
{
	return Float2({ (float)desc_.Width , (float)desc_.Height });
}

ID3D11RenderTargetView* Texture::RenderTargetView() const
{
	return pRenderTargetView_;
}

ID3D11DepthStencilView* Texture::DepthStencilView() const
{
	return pDepthStencilView_;
}

ID3D11ShaderResourceView* Texture::ShaderResourceView() const
{
	return pShaderResourceView_;
}

bool Texture::CreateRenderTargetView()
{
	if (nullptr == pTexture_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr != pRenderTargetView_)
	{
		pRenderTargetView_->Release();
		pRenderTargetView_ = nullptr;
	}

	HRESULT hr = GRenderer->Device()->CreateRenderTargetView(pTexture_, nullptr, &pRenderTargetView_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Texture::CreateDepthStencilView()
{
	if (nullptr == pTexture_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr != pDepthStencilView_)
	{
		pDepthStencilView_->Release();
		pDepthStencilView_ = nullptr;
	}
	HRESULT hr = GRenderer->Device()->CreateDepthStencilView(pTexture_, nullptr, &pDepthStencilView_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Texture::CreateShaderResourceView()
{
	if (nullptr == pTexture_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr != pShaderResourceView_)
	{
		pShaderResourceView_->Release();
		pShaderResourceView_ = nullptr;
	}
	HRESULT hr = GRenderer->Device()->CreateShaderResourceView(pTexture_, nullptr, &pShaderResourceView_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Texture::CleanUp()
{
	if (nullptr != pShaderResourceView_)
	{
		pShaderResourceView_->Release();
		pShaderResourceView_ = nullptr;
	}
	if (nullptr != pRenderTargetView_)
	{
		pRenderTargetView_->Release();
		pRenderTargetView_ = nullptr;
	}
	if (nullptr != pDepthStencilView_)
	{
		pDepthStencilView_->Release();
		pDepthStencilView_ = nullptr;
	}
	if (nullptr != pTexture_)
	{
		pTexture_->Release();
		pTexture_ = nullptr;
	}
}