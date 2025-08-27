#include "stdafx.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "BaseShader.h"

InputLayout::InputLayout()
	: refCount_(1),
	pInputLayout_(nullptr)
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


bool InputLayout::Create(IVertex* pVertex, IShader* pVertexShader)
{
	CleanUp();

	VertexBuffer* pBuffer = (VertexBuffer*)pVertex;
	BaseShader* pShader = (BaseShader*)pVertexShader;

	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layoutDesc = pBuffer->GetDesc();

	HRESULT hr = GRenderer->Device()->CreateInputLayout(layoutDesc.data(), (UINT)layoutDesc.size(), pShader->GetBufferPointer(), pShader->GetBufferSize(), &pInputLayout_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "InputLayout::pInputLayout_";
	pInputLayout_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

void InputLayout::CleanUp()
{
	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
}
