#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
	: refCount_(1)
	, vertexFormat_()
	, vertexStride_(0)
	, vertexCount_(0)
	, pVertexBuffer_(nullptr)
	, pVertices_(nullptr)
	//, meshSubsetCount_(0)
	, meshSubsets_()
	//, indexStride_(0)
	//, indexCount_(0)
	//, pIndexBuffer_(nullptr)
	//, pIndices_(nullptr)
{
}

Mesh::~Mesh()
{
	CleanUp();
}

IMesh* Mesh::Create(const MeshDesc& desc)
{
	Mesh* pNewMesh = nullptr;
	ID3D11Buffer* pVertexBuffer = nullptr;
	void* copyVertices = nullptr;

	std::vector<MeshSubset> meshSubsets;
	meshSubsets.resize(desc.meshSubsets.size());

	do
	{
		if (false == CreateVertexBuffer(&pVertexBuffer, &copyVertices, desc.vertexFormatSize, desc.vertexCount, desc.pVertices))
		{
			DEBUG_BREAK();
			break;
		}

		for (int i = 0; i < desc.meshSubsets.size(); ++i)
		{
			ID3D11Buffer* pIndexBuffer = nullptr;
			void* pCopyIndices = nullptr;
			if (false == CreateIndexBuffer(&pIndexBuffer, &pCopyIndices, desc.meshSubsets[i].indexFormatSize, desc.meshSubsets[i].indexCount, desc.meshSubsets[i].pIndices))
			{
				DEBUG_BREAK();
				break;
			}

			meshSubsets[i].materialSlot = desc.meshSubsets[i].materialSlot;
			meshSubsets[i].indexFormatSize = desc.meshSubsets[i].indexFormatSize;
			meshSubsets[i].indexCount = desc.meshSubsets[i].indexCount;
			meshSubsets[i].pIndexBuffer = pIndexBuffer;
			meshSubsets[i].pIndices = pCopyIndices;
		}
		

		pNewMesh = new Mesh;
		if (false == pNewMesh->Init
		(
			desc.vertexFormat,
			desc.vertexFormatSize,
			desc.vertexCount,
			copyVertices,
			pVertexBuffer,
			//desc.meshSubsetCount,
			meshSubsets
			//desc.indexFormatSize,
			//desc.indexCount,
			//copyIndices,
			//pIndexBuffer
		))
		{
			Assert("Mesh Init Fail !!");
			break;
		}

		return pNewMesh;

	} while (false);


	for (int i = 0; i < meshSubsets.size(); ++i)
	{
		if (nullptr != meshSubsets[i].pIndices)
		{
			free(meshSubsets[i].pIndices);
			meshSubsets[i].pIndices = nullptr;
		}

		if (nullptr != meshSubsets[i].pIndexBuffer)
		{
			meshSubsets[i].pIndexBuffer->Release();
			meshSubsets[i].pIndexBuffer = nullptr;
		}
	}
	if (nullptr != copyVertices)
	{
		free(copyVertices);
		copyVertices = nullptr;
	}
	if (nullptr != pVertexBuffer)
	{
		pVertexBuffer->Release();
		pVertexBuffer = nullptr;
	}
	if (nullptr != pNewMesh)
	{
		pNewMesh->Release();
		pNewMesh = nullptr;
	}

	return nullptr;
}

bool Mesh::Init
(
	E_VERTEX_FORMAT vertexFormat, 
	uint32_t vertexFormatSize, 
	uint32_t vertexCount, 
	void* pVertices, 
	ID3D11Buffer* pVertexBuffer, 
	//uint16_t meshSubsetCount,
	const std::vector<MeshSubset>& meshSubsets
)
{
	vertexFormat_ = vertexFormat;
	vertexStride_ = vertexFormatSize;
	vertexCount_ = vertexCount;
	pVertices_ = pVertices;
	pVertexBuffer_ = pVertexBuffer;
	//meshSubsetCount_ = meshSubsetCount;
	meshSubsets_ = meshSubsets;
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

void Mesh::BindVertices() const
{
	UINT offset = 0;
	GRenderer->DeviceContext()->IASetVertexBuffers(0, 1, &pVertexBuffer_, &vertexStride_, &offset);
	GRenderer->DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//void Mesh::Bind(uint32_t subsetIndex) const
//{
//
//
//	if (nullptr != meshSubsets_[subsetIndex].pIndexBuffer)
//	{
//		DEBUG_BREAK();
//		return;
//	}
//
//	
//	//GRenderer->DeviceContext()->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R16_UINT, 0);
//
//	
//}


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

const std::vector<MeshSubset>& Mesh::GetMeshSubsets() const
{
	return meshSubsets_;
}

//ULONG Mesh::GetIndexCount(uint32_t subSetIndex) const
//{
//	return meshSubsets_[subSetIndex].indexCount;
//	//return indexCount_;
//}

//void __stdcall Mesh::GetIndicesData
//(
//	uint32_t* pOutStride, 
//	uint32_t* pOutCount, 
//	void** ppOutIndices
//) const
//{
//	*pOutStride = indexStride_;
//	*pOutCount = indexCount_;
//	*ppOutIndices = pIndices_;
//}


bool Mesh::CreateVertexBuffer(ID3D11Buffer** ppOutVertexBuffer, void** ppOutCopyVertices, uint32_t vertexFormatSize, uint32_t vertexCount, void* pVertices)
{
	if (nullptr == pVertices)
	{
		Assert("DESC::Vertices is NULL");
		return false;
	}

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexFormatSize * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = pVertices;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, ppOutVertexBuffer);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// DeepCopy - Vertices 
	if (vertexCount != 0)
	{
		size_t dataSize = vertexCount * vertexFormatSize;
		*ppOutCopyVertices = malloc(dataSize);   // 또는 new char[dataSize];
		memcpy(*ppOutCopyVertices, pVertices, dataSize);
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "VertexBuffer";
	(*ppOutVertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

bool Mesh::CreateIndexBuffer(ID3D11Buffer** ppOutIndexBuffer, void** ppOutCopyIndices, uint32_t indexFormatSize, uint32_t indexCount, void* pIndices)
{
	if (nullptr == pIndices)
	{
		DEBUG_BREAK();
		return false;
	}

	// 인덱스 버퍼
	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = indexFormatSize * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
	InitData.pSysMem = pIndices;

	HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, ppOutIndexBuffer);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return false;
	}

	// DeepCopy - Indices 
	if (indexCount != 0)
	{
		size_t dataSize = indexCount * indexFormatSize;
		*ppOutCopyIndices = malloc(dataSize);   // 또는 new char[dataSize];
		memcpy(*ppOutCopyIndices, pIndices, dataSize);
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "IndexBuffer";
	(*ppOutIndexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

void Mesh::CleanUp()
{
	for (int i = 0; i < meshSubsets_.size(); ++i)
	{
		if (nullptr != meshSubsets_[i].pIndices)
		{
			free(meshSubsets_[i].pIndices);
			meshSubsets_[i].pIndices = nullptr;
		}
		if (nullptr != meshSubsets_[i].pIndexBuffer)
		{
			meshSubsets_[i].pIndexBuffer->Release();
			meshSubsets_[i].pIndexBuffer = nullptr;
		}
	}

	if (nullptr != pVertices_)
	{
		free(pVertices_);
		pVertices_ = nullptr;
	}
	if (nullptr != pVertexBuffer_)
	{
		pVertexBuffer_->Release();
		pVertexBuffer_ = nullptr;
	}
}