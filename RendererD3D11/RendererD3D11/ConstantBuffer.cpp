#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
	: refCount_(1),
	buffer_(nullptr) {
}

ConstantBuffer::~ConstantBuffer()
{
	Cleanup();
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

void __stdcall ConstantBuffer::Update(void* srcData)
{
	GRenderer->DeviceContext()->UpdateSubresource(buffer_, 0, nullptr, srcData, 0, 0);
}

void __stdcall ConstantBuffer::VSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->VSSetConstantBuffers(slot, 1, &buffer_);
}

void __stdcall ConstantBuffer::PSSetting(uint32_t slot)
{
	GRenderer->DeviceContext()->PSSetConstantBuffers(slot, 1, &buffer_);
}

bool ConstantBuffer::CreateBuffer(UINT bufferSize)
{
	D3D11_BUFFER_DESC desc{};
	memset(&desc, 0x00, sizeof(D3D11_BUFFER_DESC));

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = bufferSize;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&desc, nullptr, &buffer_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void ConstantBuffer::Cleanup()
{
	if (nullptr != buffer_)
	{
		buffer_->Release();
		buffer_ = nullptr;
	}
}
