#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
	: refCount_(1)
	, primitiveType_(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_POINTLIST)
	, vertexStride_(0)
	, vertexCount_(0)
	, pVertexBuffer_(nullptr)
	, pVertices_(nullptr)
	, pVerticesSRV_(nullptr)
	, pVerticesUAV_(nullptr)
	, meshSubsets_()
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
	ID3D11ShaderResourceView* pVerticesSRV = nullptr;
	ID3D11UnorderedAccessView* pVerticesUAV = nullptr;
	void* copyVertices = nullptr;

	std::vector<MeshSubset> meshSubsets;
	meshSubsets.resize(desc.meshSubsets.size());

	do
	{
		if (false == CreateVertexBuffer(&pVertexBuffer, &copyVertices, desc.vertexFormatSize, desc.vertexCount, desc.resourceFlag, desc.pVertices))
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
		

		if (static_cast<uint32_t>(desc.resourceFlag) & static_cast<uint32_t>(E_MESH_RESOURCE_FLAG::ShaderResource))
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Format = DXGI_FORMAT_UNKNOWN;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			srvDesc.Buffer.FirstElement = 0;
			srvDesc.Buffer.NumElements = desc.vertexCount;
			HRESULT hr = GRenderer->Device()->CreateShaderResourceView(pVertexBuffer, &srvDesc, &pVerticesSRV);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}

		if (static_cast<uint32_t>(desc.resourceFlag) & static_cast<uint32_t>(E_MESH_RESOURCE_FLAG::UnorderedAccess))
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
			uavDesc.Buffer.FirstElement = 0;
			uavDesc.Buffer.NumElements = desc.vertexCount;
			uavDesc.Buffer.Flags = 0;
			HRESULT hr = GRenderer->Device()->CreateUnorderedAccessView(pVertexBuffer, &uavDesc, &pVerticesUAV);
			if (FAILED(hr))
			{
				DEBUG_BREAK();
				break;
			}
		}


		pNewMesh = new Mesh;
		if (false == pNewMesh->Init
		(
			desc.primitiveType,
			desc.vertexFormatSize,
			desc.vertexCount,
			copyVertices,
			pVertexBuffer,
			pVerticesSRV,
			pVerticesUAV,
			meshSubsets
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
	if (nullptr != pVerticesSRV)
	{
		pVerticesSRV->Release();
		pVerticesSRV = nullptr;
	}
	if (nullptr != pVerticesUAV)
	{
		pVerticesUAV->Release();
		pVerticesUAV = nullptr;
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
	E_MESH_PRIMITIVE_TYPE primitiveType,
	uint32_t vertexFormatSize, 
	uint32_t vertexCount, 
	void* pVertices, 
	ID3D11Buffer* pVertexBuffer, 
	ID3D11ShaderResourceView* pVerticesSRV,
	ID3D11UnorderedAccessView* pVerticesUAV,
	const std::vector<MeshSubset>& meshSubsets
)
{	
	switch (primitiveType)
	{
	case E_MESH_PRIMITIVE_TYPE::Point:
		primitiveType_ = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case E_MESH_PRIMITIVE_TYPE::Line:
		primitiveType_ = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case E_MESH_PRIMITIVE_TYPE::Triangle:
		primitiveType_ = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	default:
		break;
	}
	
	vertexStride_ = vertexFormatSize;
	vertexCount_ = vertexCount;
	pVertices_ = pVertices;
	pVertexBuffer_ = pVertexBuffer;
	pVerticesSRV_ = pVerticesSRV;
	pVerticesUAV_ = pVerticesUAV;
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
	GRenderer->DeviceContext()->IASetPrimitiveTopology(primitiveType_);
}


void Mesh::BindUnorderedAccessViewCS(UINT slot)
{
	if (nullptr == pVerticesUAV_)
	{
		DEBUG_BREAK();
		return;
	}
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(slot, 1, &pVerticesUAV_, nullptr);
}

void Mesh::UnBindUnorderedAccessViewCS(UINT slot)
{
	ID3D11UnorderedAccessView* pNullUAV = nullptr;
	GRenderer->DeviceContext()->CSSetUnorderedAccessViews(slot, 1, &pNullUAV, nullptr);
}

void Mesh::BindShaderResourceViewVS(UINT slot)
{
	if (nullptr == pVerticesSRV_)
	{
		DEBUG_BREAK();
		return;
	}
	GRenderer->DeviceContext()->VSSetShaderResources(slot, 1, &pVerticesSRV_);
}

void Mesh::BindShaderResourceViewGS(UINT slot)
{
	if (nullptr == pVerticesSRV_)
	{
		DEBUG_BREAK();
		return;
	}
	GRenderer->DeviceContext()->GSSetShaderResources(slot, 1, &pVerticesSRV_);
}

void Mesh::BindShaderResourceViewPS(UINT slot)
{
	if (nullptr == pVerticesSRV_)
	{
		DEBUG_BREAK();
		return;
	}
	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pVerticesSRV_);
}

void Mesh::UnBindShaderResourceViewVS(UINT slot)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GRenderer->DeviceContext()->VSSetShaderResources(slot, 1, &pNullSRV);
}
void Mesh::UnBindShaderResourceViewGS(UINT slot)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GRenderer->DeviceContext()->GSSetShaderResources(slot, 1, &pNullSRV);
}
void Mesh::UnBindShaderResourceViewPS(UINT slot)
{
	ID3D11ShaderResourceView* pNullSRV = nullptr;
	GRenderer->DeviceContext()->PSSetShaderResources(slot, 1, &pNullSRV);
}


//void __stdcall Mesh::GetVerticesData
//(
//	//E_VERTEX_FORMAT* pOutFormat, 
//	uint32_t* pOutStride, 
//	uint32_t* pOutCount, 
//	void** ppOutVertices
//) const
//{
//	//*pOutFormat = vertexFormat_;
//	*pOutStride = vertexStride_;
//	*pOutCount = vertexCount_;
//	*ppOutVertices = pVertices_;
//}
//

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


bool Mesh::CreateVertexBuffer
(
	ID3D11Buffer** ppOutVertexBuffer, 
	void** ppOutCopyVertices, 
	uint32_t vertexFormatSize, 
	uint32_t vertexCount, 
	E_MESH_RESOURCE_FLAG resourceFlag,
	void* pVertices
)
{
	if (nullptr == pVertices)
	{
		Assert("DESC::Vertices is NULL");
		return false;
	}

	UINT bindFlags = 0;
	UINT miscFlags = 0;
	UINT structureByteStride = 0;
	if (resourceFlag == E_MESH_RESOURCE_FLAG::None)
	{
		bindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		if (static_cast<uint32_t>(resourceFlag) & static_cast<uint32_t>(E_MESH_RESOURCE_FLAG::ShaderResource))
		{
			bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		}
		if (static_cast<uint32_t>(resourceFlag) & static_cast<uint32_t>(E_MESH_RESOURCE_FLAG::UnorderedAccess))
		{
			bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
			miscFlags |= D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			structureByteStride = vertexFormatSize;
		}
	}


	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = vertexFormatSize * vertexCount;
	bd.BindFlags = bindFlags;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = miscFlags;
	bd.StructureByteStride = structureByteStride;

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
	if (nullptr != pVerticesSRV_)
	{
		pVerticesSRV_->Release();
		pVerticesSRV_ = nullptr;
	}
	if (nullptr != pVerticesUAV_)
	{
		pVerticesUAV_->Release();
		pVerticesUAV_ = nullptr;
	}
}