#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(ID3D11Buffer* pBuffer)
	: refCount_(1),
	pBuffer_(pBuffer)
{
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

void ConstantBuffer::CleanUp()
{
	if (nullptr != pBuffer_)
	{
		pBuffer_->Release();
		pBuffer_ = nullptr;
	}
}
