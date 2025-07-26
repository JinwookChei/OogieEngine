#include "stdafx.h"
#include "BaseShader.h"
#include "VertexShader.h"

VertexShader::VertexShader()
	:shader_(nullptr)
{
}

VertexShader::~VertexShader()
{
}

bool VertexShader::CreateShader(void* shaderByteCode, size_t byteCodeLength)
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

	HRESULT hr = GRenderer->Device()->CreateVertexShader(shaderByteCode, byteCodeLength, nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void VertexShader::SetShader()
{
	if (nullptr == shader_)
	{
		DEBUG_BREAK();
		return;
	}
	GRenderer->DeviceContext()->VSSetShader(shader_, nullptr, 0);
}

void VertexShader::CleanUp()
{
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
