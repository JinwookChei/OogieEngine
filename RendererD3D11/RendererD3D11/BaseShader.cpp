#include "stdafx.h"
#include "BaseShader.h"

D3D11BaseShader::D3D11BaseShader()
	:refCount_(1),
	pBlob_(nullptr)
{
}

D3D11BaseShader::~D3D11BaseShader()
{
	CleanUp();
}

HRESULT __stdcall D3D11BaseShader::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11BaseShader::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11BaseShader::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall D3D11BaseShader::Setting()
{
	SetShader();
}

bool D3D11BaseShader::CreateShader(ID3DBlob* pBlob)
{
	if (nullptr == pBlob)
	{
		return false;
	}

	pBlob_ = pBlob;

	pBlob_->AddRef();

	return OnCreateShader(pBlob_);
}

void* D3D11BaseShader::GetBufferPointer()
{
	if (nullptr == pBlob_)
	{
		return nullptr;
	}
	return pBlob_->GetBufferPointer();
}

size_t D3D11BaseShader::GetBufferSize()
{
	if (nullptr == pBlob_)
	{
		return 0;
	}

	return pBlob_->GetBufferSize();
}


void D3D11BaseShader::CleanUp()
{
	if (nullptr != pBlob_)
	{
		pBlob_->Release();
		pBlob_ = nullptr;
	}
}