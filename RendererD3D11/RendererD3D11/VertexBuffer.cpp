#include "stdafx.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
	: refCount_(1),
	vertexBuffer_(nullptr),
	indexBuffer_(nullptr)
{
}

VertexBuffer::~VertexBuffer()
{
	CleanUp();
}

HRESULT __stdcall VertexBuffer::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall VertexBuffer::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall VertexBuffer::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool VertexBuffer::Initialize(void* vertices, UINT vertexSize, void* indices /*= nullptr*/, UINT indexSize /*= 0*/)
{
	if (nullptr == vertices)
	{
		DEBUG_BREAK();
		return false;
	}
	if (0 == vertexSize)
	{
		DEBUG_BREAK();
		return false;
	}

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(vertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	// D3D11_SUBRESOURCE_DATA 구조체는 Direct3D 11에서 리소스(버퍼, 텍스처 등)를 생성할 때
	// 해당 리소스에 초기 데이터를 전달하는 데 사용됩니다. 
	// 즉, GPU에 올릴 자원(예: 정점 버퍼, 인덱스 버퍼, 텍스처 등)에 처음 채워넣을 
	// 데이터를 지정하는 용도입니다	
	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = vertices;								// pSysMem: 리소스 생성 시 GPU로 복사할 시스템 메모리상의 데이터 포인터입니다.

	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &vertexBuffer_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return hr;
	}

	if (nullptr == indices && 0 != indexSize)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr == indices)
	{
		return true;
	}

	if (0 == indexSize)
	{
		DEBUG_BREAK();
		return false;
	}

	// 인덱스 버퍼
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(indices);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	InitData.pSysMem = indices;

	hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, &indexBuffer_);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return hr;
	}

	return true;
}


void VertexBuffer::CleanUp()
{
	if (nullptr != vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}

	if (nullptr != indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
}