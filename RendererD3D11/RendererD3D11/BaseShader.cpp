#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader()
	:refCount_(1),
	pBlob_(nullptr)
{
}

BaseShader::~BaseShader()
{
	CleanUp();
}

HRESULT __stdcall BaseShader::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall BaseShader::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall BaseShader::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall BaseShader::Setting()
{
	SetShader();
}

bool BaseShader::CreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		return false;
	}

	pBlob_ = pBlob;

	pBlob_->AddRef();

	return OnCreateShader(pBlob_);
}

void* BaseShader::GetBufferPointer()
{
	if (nullptr == pBlob_)
	{
		return nullptr;
	}
	return pBlob_->GetBufferPointer();
}

size_t BaseShader::GetBufferSize()
{
	if (nullptr == pBlob_)
	{
		return 0;
	}

	return pBlob_->GetBufferSize();
}


void BaseShader::CleanUp()
{
	if (nullptr != pBlob_)
	{
		pBlob_->Release();
		pBlob_ = nullptr;
	}
}