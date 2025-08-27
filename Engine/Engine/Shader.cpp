#include "stdafx.h"
#include "Shader.h"

Shader::Shader()
	:pShaderImpl_(nullptr)
{
}

Shader::~Shader()
{
	CleanUp();
}

void Shader::SetShader(IShader* pShader)
{
	if (nullptr != pShaderImpl_)
	{
		pShaderImpl_->Release();
		pShaderImpl_ = nullptr;
	}

	if (nullptr != pShader)
	{
		pShaderImpl_ = pShader;
		pShaderImpl_->AddRef();
	}
}

void Shader::CleanUp()
{
	if (nullptr != pShaderImpl_)
	{
		pShaderImpl_->Release();
		pShaderImpl_ = nullptr;
	}
}
