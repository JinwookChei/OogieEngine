#pragma once

struct MeshSubset
{
	uint32_t materialSlot;
	uint32_t indexFormatSize;
	uint32_t indexCount;
	ID3D11Buffer* pIndexBuffer;
	void* pIndices;
	
	MeshSubset()
		: materialSlot(0)
		, indexFormatSize(0)
		, indexCount(0)
		, pIndexBuffer(nullptr)
		, pIndices(nullptr)
	{
	}

	void BindIndices() const
	{
		GRenderer->DeviceContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}
};

class Mesh : public IMesh
{
private:
	friend class RenderResourceFactory;
	Mesh();
	virtual ~Mesh();
	static IMesh* Create(const MeshDesc& desc);
	static bool CreateVertexBuffer
	(
		ID3D11Buffer** ppOutVertexBuffer,
		void** ppOutCopyVertices,
		uint32_t vertexFormatSize,
		uint32_t vertexCount,
		void* pVertices
	);

	static bool CreateIndexBuffer
	(
		ID3D11Buffer** ppOutIndexBuffer,
		void** ppOutCopyIndices,
		uint32_t indexFormatSize,
		uint32_t indexCount,
		void* pIndices
	);

	bool Init
	(
		E_VERTEX_FORMAT vertexFormat,
		uint32_t vertexFormatSize,
		uint32_t vertexCount,
		void* pVertices,
		ID3D11Buffer* pVertexBuffer,
		//uint16_t meshSubsetCount,
		const std::vector<MeshSubset>& meshSubsets
	);


public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void BindVertices() const;

	void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const override;
	//void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const override;

	const std::vector<MeshSubset>& GetMeshSubsets() const;

	//ULONG GetIndexCount(uint32_t subSetIndex) const;

private:

	void CleanUp();

	ULONG refCount_;

	E_VERTEX_FORMAT vertexFormat_;
	uint32_t vertexStride_;
	uint32_t vertexCount_;
	void* pVertices_;
	ID3D11Buffer* pVertexBuffer_;

	//uint16_t meshSubsetCount_;
	std::vector<MeshSubset> meshSubsets_;

	/*uint32_t indexStride_;
	uint32_t indexCount_;
	void* pIndices_;
	ID3D11Buffer* pIndexBuffer_;*/

	//UINT subMeshCount_;
	//MeshSubset* pMeshSubsets_;
};
