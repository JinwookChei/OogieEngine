#include "stdafx.h"
#include "PipelineStateObject.h"

PipelineStateObject::PipelineStateObject()
	:refCount_(1)
{
}

PipelineStateObject::~PipelineStateObject()
{
}

HRESULT __stdcall PipelineStateObject::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall PipelineStateObject::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall PipelineStateObject::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}


void PipelineStateObject::CleanUp()
{

}