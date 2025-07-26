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

	// D3D11_SUBRESOURCE_DATA ����ü�� Direct3D 11���� ���ҽ�(����, �ؽ�ó ��)�� ������ ��
	// �ش� ���ҽ��� �ʱ� �����͸� �����ϴ� �� ���˴ϴ�. 
	// ��, GPU�� �ø� �ڿ�(��: ���� ����, �ε��� ����, �ؽ�ó ��)�� ó�� ä������ 
	// �����͸� �����ϴ� �뵵�Դϴ�	
	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = vertices;								// pSysMem: ���ҽ� ���� �� GPU�� ������ �ý��� �޸𸮻��� ������ �������Դϴ�.

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

	// �ε��� ����
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