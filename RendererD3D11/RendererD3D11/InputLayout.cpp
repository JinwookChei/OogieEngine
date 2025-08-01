#include "stdafx.h"
#include "InputLayout.h"

InputLayout::InputLayout()
	: refCount_(1)
{
}

InputLayout::~InputLayout()
{
}

HRESULT __stdcall InputLayout::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall InputLayout::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall InputLayout::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall InputLayout::Setting()
{
}
