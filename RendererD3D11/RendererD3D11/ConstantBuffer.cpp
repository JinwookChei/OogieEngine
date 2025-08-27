#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
	: refCount_(1),
	pBuffer_(nullptr) {
}

ConstantBuffer::~ConstantBuffer()
{
	CleanUp();
}

HRESULT __stdcall ConstantBuffer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ConstantBuffer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall ConstantBuffer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void __stdcall ConstantBuffer::Update(void* pSrcData)
{
	GRenderer->DeviceContext()->UpdateSubresource(pBuffer_, 0, nullptr, pSrcData, 0, 0);
}

void __stdcall ConstantBuffer::VSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->VSSetConstantBuffers(slot, 1, &pBuffer_);
}

void __stdcall ConstantBuffer::PSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->PSSetConstantBuffers(slot, 1, &pBuffer_);
}

bool ConstantBuffer::CreateBuffer(UINT bufferSize)
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

void ConstantBuffer::CleanUp()
{
	if (nullptr != pBuffer_)
	{
		pBuffer_->Release();
		pBuffer_ = nullptr;
	}
}
