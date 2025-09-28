#include "stdafx.h"
#include "InputLayout.h"
#include "VertexBuffer.h"
#include "BaseShader.h"

D3D11InputLayout::D3D11InputLayout()
	: refCount_(1),
	pInputLayout_(nullptr)
{
}

D3D11InputLayout::~D3D11InputLayout()
{
	CleanUp();
}

HRESULT __stdcall D3D11InputLayout::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11InputLayout::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11InputLayout::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tmpRefCount;
}

void __stdcall D3D11InputLayout::Setting()
{
	GRenderer->DeviceContext()->IASetInputLayout(pInputLayout_);
}


bool D3D11InputLayout::Create(IVertex* pVertex, IShader* pVertexShader)
{
	CleanUp();

	D3D11VertexBuffer* pBuffer = (D3D11VertexBuffer*)pVertex;
	D3D11BaseShader* pShader = (D3D11BaseShader*)pVertexShader;

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

void D3D11InputLayout::CleanUp()
{
	if (nullptr != pInputLayout_)
	{
		pInputLayout_->Release();
		pInputLayout_ = nullptr;
	}
}
