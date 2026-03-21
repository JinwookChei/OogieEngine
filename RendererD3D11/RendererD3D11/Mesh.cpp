#include "stdafx.h"
#include "Mesh.h"

Mesh::Mesh()
	: refCount_(1)
	, vertexType_(E_VERTEX_TYPE::NONE)
	, primitiveType_(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)
	, vertexStride_(0)
	, vertexCount_(0)
	, pVertexBuffer_(nullptr)
	, pVertices_(nullptr)
	, pVerticesSRV_(nullptr)
	, pVerticesUAV_(nullptr)
	, subMeshes_()
	, aabb_()
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
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
	std::vector<SubMesh> subMeshes;
	subMeshes.resize(desc.subMeshDesc.size());

	do
	{
		if (false == CreateVertexBuffer(&pVertexBuffer, &copyVertices, desc.bufferSize, desc.vertexFormatSize, desc.vertexCount, desc.usage, desc.bindFlag, desc.pVertices))
		{
			DEBUG_BREAK();
			break;
		}

		for (int i = 0; i < desc.subMeshDesc.size(); ++i)
		{
			ID3D11Buffer* pIndexBuffer = nullptr;
			void* pCopyIndices = nullptr;
			if (false == CreateIndexBuffer(&pIndexBuffer, &pCopyIndices, desc.subMeshDesc[i].indexCount, desc.subMeshDesc[i].pIndices))
			{
				DEBUG_BREAK();
				break;
			}

			subMeshes[i].materialSlot = desc.subMeshDesc[i].materialSlot;
			subMeshes[i].indexCount = desc.subMeshDesc[i].indexCount;
			subMeshes[i].pIndexBuffer = pIndexBuffer;
			subMeshes[i].pIndices = pCopyIndices;
		}

		if ((desc.bindFlag & E_MESH_BIND_FLAG::SHADER_RESOURCE) != E_MESH_BIND_FLAG::NONE)
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

		if ((desc.bindFlag & E_MESH_BIND_FLAG::UNORDERED_ACCESS) != E_MESH_BIND_FLAG::NONE)
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

		switch (desc.primitiveType)
		{
		case E_MESH_PRIMITIVE_TYPE::POINT:
			primitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			break;
		case E_MESH_PRIMITIVE_TYPE::LINE:
			primitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			break;
		case E_MESH_PRIMITIVE_TYPE::TRIANGLE:
			primitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			break;
		default:
			break;
		}
		pNewMesh = new Mesh;
		if (false == pNewMesh->Init
		(
			primitiveType,
			desc.vertexType,
			desc.vertexFormatSize,
			desc.vertexCount,
			copyVertices,
			pVertexBuffer,
			pVerticesSRV,
			pVerticesUAV,
			subMeshes
		))
		{
			Assert("Mesh Init Fail !!");
			break;
		}

		return pNewMesh;

	} while (false);


	for (int i = 0; i < subMeshes.size(); ++i)
	{
		if (nullptr != subMeshes[i].pIndices)
		{
			free(subMeshes[i].pIndices);
			subMeshes[i].pIndices = nullptr;
		}

		if (nullptr != subMeshes[i].pIndexBuffer)
		{
			subMeshes[i].pIndexBuffer->Release();
			subMeshes[i].pIndexBuffer = nullptr;
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
	D3D_PRIMITIVE_TOPOLOGY primitiveType,
	E_VERTEX_TYPE vertexType,
	uint32_t vertexFormatSize,
	uint32_t vertexCount,
	void* pVertices,
	ID3D11Buffer* pVertexBuffer,
	ID3D11ShaderResourceView* pVerticesSRV,
	ID3D11UnorderedAccessView* pVerticesUAV,
	const std::vector<SubMesh>& subMeshes
)
{
	primitiveType_ = primitiveType;
	vertexType_ = vertexType;
	vertexStride_ = vertexFormatSize;
	vertexCount_ = vertexCount;
	pVertices_ = pVertices;
	pVertexBuffer_ = pVertexBuffer;
	pVerticesSRV_ = pVerticesSRV;
	pVerticesUAV_ = pVerticesUAV;
	subMeshes_ = subMeshes;
	CalcAABB();

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


void __stdcall Mesh::WriteBuffer(const void* data, uint32_t size)
{
	D3D11_BUFFER_DESC desc;
	pVertexBuffer_->GetDesc(&desc);
	if (desc.Usage != D3D11_USAGE_DYNAMIC || !(desc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE))
	{
		DEBUG_BREAK();
		return;
	}
	if (size > desc.ByteWidth)
	{
		DEBUG_BREAK();
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mapped = {};
	HRESULT hr = GRenderer->DeviceContext()->Map(pVertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (FAILED(hr))
	{
		DEBUG_BREAK();
		return;
	}
	memcpy(mapped.pData, data, size);
	GRenderer->DeviceContext()->Unmap(pVertexBuffer_, 0);

	vertexCount_ = size / vertexStride_;
}

AABB __stdcall Mesh::GetAABB()
{
	return aabb_;
}

void __stdcall Mesh::GetVerticesData(void** ppOutVertices, E_VERTEX_TYPE* pOutType) const
{
	*ppOutVertices = pVertices_;
	*pOutType = vertexType_;
}

void __stdcall Mesh::GetIndiciesData(void** ppOutIndices, uint32_t* pOutIndicesCount, uint32_t subMeshIndex) const
{
	if (subMeshIndex >= subMeshes_.size())
	{
		DEBUG_BREAK();
		return;
	}
	
	*pOutIndicesCount = subMeshes_[subMeshIndex].indexCount;
	*ppOutIndices = subMeshes_[subMeshIndex].pIndices;
}

uint32_t __stdcall Mesh::GetSubMeshCount() const
{
	return subMeshes_.size();
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

const std::vector<SubMesh>& Mesh::GetSubMeshes() const
{
	return subMeshes_;
}

uint32_t Mesh::GetVertexCount() const
{
	return vertexCount_;
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
	uint32_t bufferSize,
	uint32_t vertexFormatSize,
	uint32_t vertexCount,
	E_MESH_USAGE usage,
	E_MESH_BIND_FLAG bindFlag,
	void* pVertices
)
{
	D3D11_USAGE usageType = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	UINT cpuAccess = 0;
	UINT bindFlags = 0;
	UINT miscFlags = 0;
	UINT structureByteStride = 0;

	switch (usage)
	{
	case E_MESH_USAGE::DEFAULT:
		usageType = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		break;
	case E_MESH_USAGE::IMMUTABLE:
		usageType = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
		break;
	case E_MESH_USAGE::DYNAMIC:
		usageType = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		cpuAccess |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		break;
	case E_MESH_USAGE::STAGING:
		usageType = D3D11_USAGE::D3D11_USAGE_STAGING;
		cpuAccess |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
		break;
	default:
		DEBUG_BREAK();
		break;
	}

	if ((bindFlag & E_MESH_BIND_FLAG::VERTEX_BUFFER) != E_MESH_BIND_FLAG::NONE)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	}
	if ((bindFlag & E_MESH_BIND_FLAG::INDEX_BUFFER) != E_MESH_BIND_FLAG::NONE)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	}
	if ((bindFlag & E_MESH_BIND_FLAG::CONSTANT_BUFFER) != E_MESH_BIND_FLAG::NONE)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	}
	if ((bindFlag & E_MESH_BIND_FLAG::SHADER_RESOURCE) != E_MESH_BIND_FLAG::NONE)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	}
	if ((bindFlag & E_MESH_BIND_FLAG::UNORDERED_ACCESS) != E_MESH_BIND_FLAG::NONE)
	{
		bindFlags |= D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
		miscFlags |= D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		structureByteStride = vertexFormatSize;
	}

	D3D11_BUFFER_DESC bd;
	memset(&bd, 0x00, sizeof(D3D11_BUFFER_DESC));
	bd.Usage = usageType;
	bd.BindFlags = bindFlags;
	bd.ByteWidth = bufferSize;
	bd.CPUAccessFlags = cpuAccess;
	bd.MiscFlags = miscFlags;
	bd.StructureByteStride = structureByteStride;

	if (nullptr == pVertices && usageType == D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, nullptr, ppOutVertexBuffer);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			return false;
		}
	}
	else if (nullptr == pVertices && usageType != D3D11_USAGE::D3D11_USAGE_DYNAMIC)
	{
		DEBUG_BREAK();
		return false;
	}
	else
	{
		D3D11_SUBRESOURCE_DATA InitData;
		memset(&InitData, 0x00, sizeof(D3D11_SUBRESOURCE_DATA));
		InitData.pSysMem = pVertices;
		HRESULT hr = GRenderer->Device()->CreateBuffer(&bd, &InitData, ppOutVertexBuffer);
		if (FAILED(hr))
		{
			DEBUG_BREAK();
			return false;
		}
	}

	// DeepCopy - Vertices 
	if (nullptr != pVertices)
	{
		UINT dataSize = bd.ByteWidth;
		*ppOutCopyVertices = malloc(dataSize);   // 또는 new char[dataSize];
		memcpy(*ppOutCopyVertices, pVertices, dataSize);
	}
	
	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "VertexBuffer";
	(*ppOutVertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

bool Mesh::CreateIndexBuffer(ID3D11Buffer** ppOutIndexBuffer, void** ppOutCopyIndices, uint32_t indexCount, void* pIndices)
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
	bd.ByteWidth = sizeof(uint32_t) * indexCount;
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
		size_t dataSize = indexCount * sizeof(uint32_t);
		*ppOutCopyIndices = malloc(dataSize);  
		memcpy(*ppOutCopyIndices, pIndices, dataSize);
	}

	// ---------------- 메모리 누수 디버깅용 이름 설정. ----------------------------
	const char* debugObjectName = "IndexBuffer";
	(*ppOutIndexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)strlen(debugObjectName), debugObjectName);
	// ---------------------------------------------------------------------------

	return true;
}

void Mesh::CalcAABB()
{
	switch (vertexType_)
	{
	case E_VERTEX_TYPE::NONE:
		break;
	case E_VERTEX_TYPE::SCREEN_QUAD:
	{
		ScreenQuadVertex* pScreenRectVertices = static_cast<ScreenQuadVertex*>(pVertices_);
		aabb_.minPos_ = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		aabb_.maxPos_ = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		for (int i = 1; i < vertexCount_; ++i)
		{
			const ScreenQuadVertex& curVertex = pScreenRectVertices[i];
			aabb_.minPos_.X = min(aabb_.minPos_.X, curVertex.position.X);
			aabb_.maxPos_.X = max(aabb_.maxPos_.X, curVertex.position.X);
			aabb_.minPos_.Y = min(aabb_.minPos_.Y, curVertex.position.Y);
			aabb_.maxPos_.Y = max(aabb_.maxPos_.Y, curVertex.position.Y);
		}
	}break;
	case E_VERTEX_TYPE::DEBUG_LINE:
		break;
	case E_VERTEX_TYPE::SIMPLE_VERTEX:
	{
		SimpleVertex* pSimpleVertices = static_cast<SimpleVertex*>(pVertices_);
		aabb_.minPos_ = pSimpleVertices[0].position;
		aabb_.maxPos_ = pSimpleVertices[0].position;
		for (int i = 1; i < vertexCount_; ++i)
		{
			const SimpleVertex& curVertex = pSimpleVertices[i];
			aabb_.minPos_.X = min(aabb_.minPos_.X, curVertex.position.X);
			aabb_.maxPos_.X = max(aabb_.maxPos_.X, curVertex.position.X);
			aabb_.minPos_.Y = min(aabb_.minPos_.Y, curVertex.position.Y);
			aabb_.maxPos_.Y = max(aabb_.maxPos_.Y, curVertex.position.Y);
			aabb_.minPos_.Z = min(aabb_.minPos_.Z, curVertex.position.Z);
			aabb_.maxPos_.Z = max(aabb_.maxPos_.Z, curVertex.position.Z);
		}
	} break;
	case E_VERTEX_TYPE::SKINNED_MESH:
	{
		SkinnedMeshVertex* pSkinnedVertices = static_cast<SkinnedMeshVertex*>(pVertices_);
		aabb_.minPos_ = pSkinnedVertices[0].position;
		aabb_.maxPos_ = pSkinnedVertices[0].position;
		for (int i = 1; i < vertexCount_; ++i)
		{
			const SkinnedMeshVertex& curVertex = pSkinnedVertices[i];
			aabb_.minPos_.X = min(aabb_.minPos_.X, curVertex.position.X);
			aabb_.maxPos_.X = max(aabb_.maxPos_.X, curVertex.position.X);
			aabb_.minPos_.Y = min(aabb_.minPos_.Y, curVertex.position.Y);
			aabb_.maxPos_.Y = max(aabb_.maxPos_.Y, curVertex.position.Y);
			aabb_.minPos_.Z = min(aabb_.minPos_.Z, curVertex.position.Z);
			aabb_.maxPos_.Z = max(aabb_.maxPos_.Z, curVertex.position.Z);
		}
	}break;
	default:
		break;
	}
	//aabb_.minPos_ = 


	//pVertices_;
}

void Mesh::CleanUp()
{
	for (int i = 0; i < subMeshes_.size(); ++i)
	{
		if (nullptr != subMeshes_[i].pIndices)
		{
			free(subMeshes_[i].pIndices);
			subMeshes_[i].pIndices = nullptr;
		}
		if (nullptr != subMeshes_[i].pIndexBuffer)
		{
			subMeshes_[i].pIndexBuffer->Release();
			subMeshes_[i].pIndexBuffer = nullptr;
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