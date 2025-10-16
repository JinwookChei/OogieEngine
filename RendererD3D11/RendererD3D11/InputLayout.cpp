#include "stdafx.h"
#include "Mesh.h"
#include "VertexShader.h"
#include "InputLayout.h"

InputLayout::InputLayout(ID3D11InputLayout* pInputLayout)
	: refCount_(1),
	pInputLayout_(pInputLayout)
{
}

InputLayout::~InputLayout()
{
	CleanUp();
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
	GRenderer->DeviceContext()->IASetInputLayout(pInputLayout_);
}

void InputLayout::CleanUp()
{
	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
}
