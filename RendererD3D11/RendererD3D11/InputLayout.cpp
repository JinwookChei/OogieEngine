#include "stdafx.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "BaseShader.h"

InputLayout::InputLayout()
	: refCount_(1),
	inputLayout_(nullptr)
{
}

InputLayout::~InputLayout()
{
	Cleanup();
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

bool InputLayout::Create(IVertex* vertex, IShader* vertexShader)
{
	Cleanup();

	VertexBuffer* buffer = (VertexBuffer*)vertex;
	BaseShader* shader = (BaseShader*)vertexShader;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc = buffer->GetDesc();

	HRESULT hr = GRenderer->Device()->CreateInputLayout(&layoutDesc[0], (UINT)layoutDesc.size(), shader->GetBufferPointer(), shader->GetBufferSize(), &inputLayOut_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}


void __stdcall InputLayout::Setting()
{
	GRenderer->DeviceContext()->IASetInputLayout(inputLayout_);
}


void InputLayout::Cleanup()
{
	if (nullptr != inputLayout_)
	{
		inputLayout_->Release();
		inputLayout_ = nullptr;
	}
}
