#include "stdafx.h"
#include "ShaderConstants.h"

ShaderConstants::ShaderConstants()
	:pConstantBufferImpl_(nullptr)
{
}

ShaderConstants::ShaderConstants(IConstantBuffer* pConstantBuffer)
	:pConstantBufferImpl_(nullptr)
{
	SetConstantBuffer(pConstantBuffer);
}

ShaderConstants::~ShaderConstants()
{
}

void ShaderConstants::Update(void* pSrcData)
{
	if (nullptr == pConstantBufferImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pConstantBufferImpl_->Update(pSrcData);
}

void ShaderConstants::VSSetting(uint32_t slot)
{
	if (nullptr == pConstantBufferImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pConstantBufferImpl_->VSSetting(slot);
}

void ShaderConstants::PSSetting(uint32_t slot)
{
	if (nullptr == pConstantBufferImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pConstantBufferImpl_->PSSetting(slot);
}


void ShaderConstants::SetConstantBuffer(IConstantBuffer* pConstantBuffer)
{
	if (nullptr != pConstantBufferImpl_)
	{
		pConstantBufferImpl_->Release();
		pConstantBufferImpl_ = nullptr;
	}

	if (nullptr != pConstantBuffer)
	{
		pConstantBufferImpl_ = pConstantBuffer;
		pConstantBufferImpl_->AddRef();
	}
}


void ShaderConstants::CleanUp()
{
	if (nullptr != pConstantBufferImpl_)
	{
		pConstantBufferImpl_->Release();
		pConstantBufferImpl_ = nullptr;
	}
}