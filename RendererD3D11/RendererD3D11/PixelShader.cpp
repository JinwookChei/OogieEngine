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

bool PixelShader::CreateShader(void* shaderByteCode, size_t byteCodeLength)
{
	if (nullptr == shaderByteCode)
	{
		DEBUG_BREAK();
		return false;
	}
	if (0 == byteCodeLength)
	{
		DEBUG_BREAK();
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreatePixelShader(shaderByteCode, byteCodeLength, nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
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
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
