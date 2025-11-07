#include "stdafx.h"
#include "Texture.h"

Texture::Texture()
	: refCount_(1),
	pTexture_(nullptr),
	pRenderTargetView_(nullptr),
	pDepthStencilView_(nullptr),
	pShaderResourceView_(nullptr)
	//desc_()
{

}

Texture::~Texture()
{
	CleanUp();
}

bool Texture::Init(ID3D11Texture2D* pTexture)
{
	return InitTexture(pTexture);
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

//Texture* Texture::Create(const Float2& size, DXGI_FORMAT format, uint32_t flag)
//{
//	D3D11_TEXTURE2D_DESC desc = { 0, };
//	desc.ArraySize = 1;
//	desc.Width = (UINT)size.X;
//	desc.Height = (UINT)size.Y;
//	desc.Format = format;
//	desc.SampleDesc.Count = 1;
//	desc.SampleDesc.Quality = 0;
//	desc.MipLevels = 1;
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.CPUAccessFlags = 0;
//	desc.BindFlags = flag;
//
//	return Create(desc);
//}

//Texture* Texture::Create(const D3D11_TEXTURE2D_DESC& desc)
//{
//	ID3D11Texture2D* texture;
//
//	HRESULT hr = GRenderer->Device()->CreateTexture2D(&desc, nullptr, &texture);
//	if (FAILED(hr))
//	{
//		DEBUG_BREAK();
//		return nullptr;
//	}
//
//	Texture* newTexture = new Texture;
//	bool ret = newTexture->SetTexture(texture);
//	if (ret == false)
//	{
//		DEBUG_BREAK();
//		return nullptr;
//	}
//
//	return newTexture;
//}


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

	return InitTexture(pTexture);
}

Float2 Texture::Size() const
{
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture_->GetDesc(&texDesc);

	return Float2({ (float)texDesc.Width , (float)texDesc.Height });
}

bool Texture::InitTexture(ID3D11Texture2D* pTexture)
{
	pTexture_ = pTexture;
	if (nullptr == pTexture_)
	{
		return true;
	}

	//pTexture_->GetDesc(&desc_);
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture_->GetDesc(&texDesc);

	if (D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET & texDesc.BindFlags)
	{
		bool ret = CreateRenderTargetView();
		if (false == ret)
		{
			return false;
		}
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL & texDesc.BindFlags)
	{
		bool ret = CreateDepthStencilView();
		if (false == ret)
		{
			return false;
		}
	}

	if (D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE & texDesc.BindFlags)
	{
		bool ret = CreateShaderResourceView();
		if (false == ret)
		{
			return false;
		}
	}

	return true;
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
	
	
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture_->GetDesc(&texDesc);
	if (DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS == texDesc.Format)
	{
		DSVDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	}
	else if (DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS == texDesc.Format)
	{
		DSVDesc.Format = DXGI_FORMAT_D32_FLOAT;
	}
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;

	HRESULT hr = GRenderer->Device()->CreateDepthStencilView(pTexture_, &DSVDesc, &pDepthStencilView_);
	if (FAILED(hr))
	{
		Assert("CreateDepthStencilView() FAIL");
		return false;
	}

	return true;
}

bool Texture::CreateShaderResourceView()
{
	if (nullptr == pTexture_)
	{
		Assert("CreateShaderResourceView::pTexture_ = NULL");
		return false;
	}

	if (nullptr != pShaderResourceView_)
	{
		pShaderResourceView_->Release();
		pShaderResourceView_ = nullptr;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC* pTmpDesc = nullptr;
	D3D11_SHADER_RESOURCE_VIEW_DESC sd = {};
	D3D11_TEXTURE2D_DESC texDesc;
	pTexture_->GetDesc(&texDesc);

	if (nullptr != pDepthStencilView_)
	{
		if (DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS == texDesc.Format)
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			sd.Format = DXGI_FORMAT::DXGI_FORMAT_R24_UNORM_X8_TYPELESS;			//R24(UNORM - 0~1 사이의 정규화된 값) bit, X8(타입미정-shader에서 접근하지 않음)
			sd.Texture2D.MostDetailedMip = 0;
			sd.Texture2D.MipLevels = 1;
			pTmpDesc = &sd;
		}
		else if (DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS == texDesc.Format)
		{
			sd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			sd.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			sd.Texture2D.MostDetailedMip = 0;
			sd.Texture2D.MipLevels = 1;
			pTmpDesc = &sd;
		}
	}

	// 이 Texture가 Depth면 FormatType 계산이 들어가고,
	// 이 Texture가 Depth가 아니면 nullptr Desc를 파라미터로 넘김.
	HRESULT hr = GRenderer->Device()->CreateShaderResourceView(pTexture_, pTmpDesc, &pShaderResourceView_);
	if (FAILED(hr))
	{
		Assert("CreateShaderResourceView()::CreateShaderResourceView = FAIL");
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