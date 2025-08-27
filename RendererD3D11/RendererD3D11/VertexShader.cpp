#include "stdafx.h"
#include "VertexShader.h"

VertexShader::VertexShader()
	:shader_(nullptr)
{
}

VertexShader::~VertexShader()
{
	CleanUp();
}

void VertexShader::SetShader()
{
	GRenderer->DeviceContext()->VSSetShader(shader_, nullptr, 0);
}

bool VertexShader::OnCreateShader(ID3DBlob* blob)
{
	if (nullptr == blob)
	{
		return false;
	}

	HRESULT hr = GRenderer->Device()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void VertexShader::CleanUp()
{
	if (nullptr != shader_)
	{
		shader_->Release();
		shader_ = nullptr;
	}
}
