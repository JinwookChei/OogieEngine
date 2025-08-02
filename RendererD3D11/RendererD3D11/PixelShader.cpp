#include "stdafx.h"
#include "PixelShader.h"

PixelShader::PixelShader()
	: shader_(nullptr)
{
}

PixelShader::~PixelShader()
{
	CleanUp();
}

bool PixelShader::CreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		DEBUG_BREAK();
		return false;
	}

	pBlob_ = pBlob;

	HRESULT hr = GRenderer->Device()->CreatePixelShader(pBlob_->GetBufferPointer(), pBlob_->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		pBlob_->Release();
		pBlob_ = nullptr;
		return false;
	}

	return true;
}

void PixelShader::SetShader()
{
	if (nullptr == shader_)
	{
		DEBUG_BREAK();
		return;
	}

	GRenderer->DeviceContext()->PSSetShader(shader_, nullptr, 0);
}

void PixelShader::CleanUp()
{
	if (nullptr != pBlob_)
	{
		pBlob_->Release();
		pBlob_ = nullptr;
	}

	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
