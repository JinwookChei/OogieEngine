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

void PixelShader::SetShader()
{
	GRenderer->DeviceContext()->PSSetShader(shader_, nullptr, 0);
}

bool PixelShader::OnCreateShader(ID3DBlob* blob)
{
	if (nullptr == blob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void PixelShader::CleanUp()
{
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
