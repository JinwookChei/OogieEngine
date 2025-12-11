#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
	: refCount_(1),
	vertexFormat_(),
	vertexStride_(0),
	vertexCount_(0),
	pVertexBuffer_(nullptr),
	pVertices_(nullptr),	
	indexStride_(0),
	indexCount_(0),
	pIndexBuffer_(nullptr),
	pIndices_(nullptr)
{
}

Mesh::~Mesh()
{
	CleanUp();
}


bool Mesh::Init
(
	E_VERTEX_FORMAT vertexFormat, 
	uint32_t vertexFormatSize, 
	uint32_t vertexCount, 
	void* pVertices, 
	ID3D11Buffer* pVertexBuffer, 
	uint32_t indexFormatSize, 
	uint32_t indexCount, 
	void* pIndices, 
	ID3D11Buffer* pIndexBuffer
)
{
	vertexFormat_ = vertexFormat;
	vertexStride_ = vertexFormatSize;
	vertexCount_ = vertexCount;
	pVertices_ = pVertices;
	pVertexBuffer_ = pVertexBuffer;

	indexStride_ = indexFormatSize;
	indexCount_ = indexCount;
	pIndices_ = pIndices;
	pIndexBuffer_ = pIndexBuffer;

	return true;
}

HRESULT __stdcall Mesh::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall Mesh::AddRef()
{
	return ++refCount_;
}

ULONG __stdcall Mesh::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

void Mesh::Bind() const
{
	UINT offset = 0;

	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &vertexStride_, &offset);

	GRenderer->DeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);

	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void __stdcall Mesh::GetVerticesData
(
	E_VERTEX_FORMAT* pOutFormat, 
	uint32_t* pOutStride, 
	uint32_t* pOutCount, 
	void** ppOutVertices
) const
{
	*pOutFormat = vertexFormat_;
	*pOutStride = vertexStride_;
	*pOutCount = vertexCount_;
	*ppOutVertices = pVertices_;
}

ULONG Mesh::GetIndexCount() const
{
	return indexCount_;
}

void __stdcall Mesh::GetIndicesData
(
	uint32_t* pOutStride, 
	uint32_t* pOutCount, 
	void** ppOutIndices
) const
{
	*pOutStride = indexStride_;
	*pOutCount = indexCount_;
	*ppOutIndices = pIndices_;
}

void Mesh::CleanUp()
{
	if (nullptr != pVertices_)
	{
		free(pVertices_);
		pVertices_ = nullptr;
	}
	if (nullptr != pIndices_)
	{
		free(pIndices_);
		pIndices_ = nullptr;
	}
	if (nullptr != pIndexBuffer_)
	{
		pIndexBuffer_->Release();
		pIndexBuffer_ = nullptr;
	}
	if (nullptr != pVertexBuffer_)
	{
		pVertexBuffer_->Release();
		pVertexBuffer_ = nullptr;
	}
}