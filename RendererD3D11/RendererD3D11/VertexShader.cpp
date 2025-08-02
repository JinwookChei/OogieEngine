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

bool VertexShader::CreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		DEBUG_BREAK();
		return false;
	}

	pBlob_ = pBlob;

	HRESULT hr = GRenderer->Device()->CreateVertexShader(pBlob_->GetBufferPointer(), pBlob_->GetBufferSize(), nullptr, &shader_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		pBlob_->Release();
		pBlob_ = nullptr;
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
