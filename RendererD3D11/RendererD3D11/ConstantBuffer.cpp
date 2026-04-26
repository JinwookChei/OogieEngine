#include "stdafx.h"
#include "ConstantBuffer.h"

ConstantBuffer* ConstantBuffer::GConstantPerFrame = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerObject = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerLight = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerComputeParticle = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerRenderParticle = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerAnimation = nullptr;
ConstantBuffer* ConstantBuffer::GConstantPerMaterial = nullptr;

ConstantBuffer::ConstantBuffer()
	: refCount_(1),
	pBuffer_(nullptr)
{
}

ConstantBuffer::~ConstantBuffer()
{
	CleanUp();
}

bool ConstantBuffer::Init(ID3D11Buffer* pBuffer)
{
	if (nullptr == pBuffer)
	{
		DEBUG_BREAK();
		return false;
	}

	pBuffer_ = pBuffer;
	return true;
}

bool ConstantBuffer::Init(uint32_t bufferSize)
{
	ID3D11Buffer* pBuffer = nullptr;

	do
	{
		D3D11_BUFFER_DESC desc{};
		memset(&desc, 0x00, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = bufferSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = 0;

		HRESULT hr = GRenderer->Device()->CreateBuffer(&desc, nullptr, &pBuffer);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			break;
		}

		// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
		const char* debugObjectName = "ConstantBuffer::pBuffer_";
		pBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
		// ---------------------------------------------------------------------------

		pBuffer_ = pBuffer;

		return true;

	} while (false);


	if (nullptr != pBuffer)
	{
		pBuffer->Release();
		pBuffer = nullptr;
	}

	return false;
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

void ConstantBuffer::Update(void* pSrcData)
{
	GRenderer->DeviceContext()->UpdateSubresource(pBuffer_, 0, nullptr, pSrcData, 0, 0);
}

void ConstantBuffer::BindConstantBufferCS(UINT slot)
{
	GRenderer->DeviceContext()->CSSetConstantBuffers(slot, 1, &pBuffer_);
}

void ConstantBuffer::BindConstantBufferVS(UINT slot)
{
	GRenderer->DeviceContext()->VSSetConstantBuffers(slot, 1, &pBuffer_);
}

void ConstantBuffer::BindConstantBufferGS(UINT slot)
{
	GRenderer->DeviceContext()->GSSetConstantBuffers(slot, 1, &pBuffer_);
}

void ConstantBuffer::BindConstantBufferPS(UINT slot)
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

void ConstantBuffer::InitGlobalConstant()
{
	ConstantBuffer::GConstantPerFrame = Create(sizeof(CBPerFrame));
	ConstantBuffer::GConstantPerObject = Create(sizeof(CBPerObject));
	ConstantBuffer::GConstantPerLight = Create(sizeof(CBPerLight));
	ConstantBuffer::GConstantPerComputeParticle = Create(sizeof(CBPerComputeParticle));
	ConstantBuffer::GConstantPerRenderParticle = Create(sizeof(CBPerParticle));
	ConstantBuffer::GConstantPerAnimation = Create(sizeof(CBPerAnimation));
	ConstantBuffer::GConstantPerMaterial = Create(sizeof(CBPerMaterial));
}

void ConstantBuffer::ShutDown()
{
	if (nullptr != ConstantBuffer::GConstantPerAnimation)
	{
		ConstantBuffer::GConstantPerAnimation->Release();
		ConstantBuffer::GConstantPerAnimation = nullptr;
	}
	if (nullptr != ConstantBuffer::GConstantPerComputeParticle)
	{
		ConstantBuffer::GConstantPerComputeParticle->Release();
		ConstantBuffer::GConstantPerComputeParticle = nullptr;
	}
	if (nullptr != ConstantBuffer::GConstantPerRenderParticle)
	{
		ConstantBuffer::GConstantPerRenderParticle->Release();
		ConstantBuffer::GConstantPerRenderParticle = nullptr;
	}
	if (nullptr != ConstantBuffer::GConstantPerFrame)
	{
		ConstantBuffer::GConstantPerFrame->Release();
		ConstantBuffer::GConstantPerFrame = nullptr;
	}

	if (nullptr != ConstantBuffer::GConstantPerObject)
	{
		ConstantBuffer::GConstantPerObject->Release();
		ConstantBuffer::GConstantPerObject = nullptr;
	}

	if (nullptr != ConstantBuffer::GConstantPerLight)
	{
		ConstantBuffer::GConstantPerLight->Release();
		ConstantBuffer::GConstantPerLight = nullptr;
	}

	if (nullptr != ConstantBuffer::GConstantPerMaterial)
	{
		ConstantBuffer::GConstantPerMaterial->Release();
		ConstantBuffer::GConstantPerMaterial = nullptr;
	}
}

ConstantBuffer* ConstantBuffer::Create(uint32_t bufferSize)
{
	ConstantBuffer* pNewConstantBuffer = new ConstantBuffer;
	if (false == pNewConstantBuffer->Init(bufferSize))
	{
		DEBUG_BREAK();
		pNewConstantBuffer->Release();
		pNewConstantBuffer = nullptr;
	}

	return pNewConstantBuffer;
}
