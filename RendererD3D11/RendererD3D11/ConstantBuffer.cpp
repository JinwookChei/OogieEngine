#include "stdafx.h"
#include "ConstantBuffer.h"

D3D11ConstantBuffer::D3D11ConstantBuffer()
	: refCount_(1),
	pBuffer_(nullptr) {
}

D3D11ConstantBuffer::~D3D11ConstantBuffer()
{
	CleanUp();
}

HRESULT __stdcall D3D11ConstantBuffer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall D3D11ConstantBuffer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall D3D11ConstantBuffer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall D3D11ConstantBuffer::Update(void* pSrcData)
{
	GRenderer->DeviceContext()->UpdateSubresource(pBuffer_, 0, nullptr, pSrcData, 0, 0);
}

void __stdcall D3D11ConstantBuffer::VSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->VSSetConstantBuffers(slot, 1, &pBuffer_);
}

void __stdcall D3D11ConstantBuffer::PSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->PSSetConstantBuffers(slot, 1, &pBuffer_);
}

bool D3D11ConstantBuffer::CreateBuffer(UINT bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	memset(&desc, 0x00, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = bufferSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&desc, nullptr, &pBuffer_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "ConstantBuffer::pBuffer_";
	pBuffer_->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------


	return true;
}

void D3D11ConstantBuffer::CleanUp()
{
	if (nullptr != pBuffer_)
	{
		pBuffer_->Release();
		pBuffer_ = nullptr;
	}
}
