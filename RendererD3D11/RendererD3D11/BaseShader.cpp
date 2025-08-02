#include "stdafx.h"
#include "BaseShader.h"

BaseShader::BaseShader()
	: refCount_(1),
	pBlob_(nullptr)
{
}

BaseShader::~BaseShader()
{
	//CleanUp();
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
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall BaseShader::Setting()
{
	SetShader();
}

ID3DBlob* BaseShader::GetBlob() const
{
	return pBlob_;
}
