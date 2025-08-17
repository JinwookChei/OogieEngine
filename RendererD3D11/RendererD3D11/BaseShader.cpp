#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader()
	:refCount_(1),
	blob_(nullptr)
{
}

BaseShader::~BaseShader()
{

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

bool BaseShader::CreateShader(ID3DBlob* blob) 
{
	if (nullptr == blob)
	{
		return false;
	}

	blob_ = blob;

	blob_->AddRef();

	return OnCreateShader(blob_);
}

void* BaseShader::GetBufferPointer()
{
	if (nullptr == blob_)
	{
		return nullptr;
	}
	return blob_->GetBufferPointer();
}

size_t BaseShader::GetBufferSize()
{
	if (nullptr == blob_)
	{
		return 0;
	}

	return blob_->GetBufferSize();
}

void BaseShader::Cleanup()
{
	OnCleanup();

	if (nullptr != blob_)
	{
		blob_->Release();
		blob_ = nullptr;
	}
}


