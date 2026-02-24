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
		E_MESH_RESOURCE_FLAG resourceFlag,
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
		E_MESH_PRIMITIVE_TYPE primitiveType,
		//E_MESH_RESOURCE_FLAG resourceFlag,
		uint32_t vertexFormatSize,
		uint32_t vertexCount,
		void* pVertices,
		ID3D11Buffer* pVertexBuffer,
		ID3D11ShaderResourceView* pVerticesSRV,
		ID3D11UnorderedAccessView* pVerticesUAV,
		const std::vector<MeshSubset>& meshSubsets
	);


public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void BindVertices() const;

	//void BindShaderResourceViewCS(UINT slot);

	void BindUnorderedAccessViewCS(UINT slot);
	void UnBindUnorderedAccessViewCS(UINT slot);

	void BindShaderResourceViewVS(UINT slot);
	void BindShaderResourceViewGS(UINT slot);
	void BindShaderResourceViewPS(UINT slot);

	void UnBindShaderResourceViewVS(UINT slot);
	void UnBindShaderResourceViewGS(UINT slot);
	void UnBindShaderResourceViewPS(UINT slot);


	//void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const override;
	//void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const override;

	const std::vector<MeshSubset>& GetMeshSubsets() const;

private:

	void CleanUp();

	ULONG refCount_;
	D3D_PRIMITIVE_TOPOLOGY primitiveType_;

	uint32_t vertexStride_;
	uint32_t vertexCount_;
	void* pVertices_;
	ID3D11Buffer* pVertexBuffer_;
	ID3D11ShaderResourceView* pVerticesSRV_;
	ID3D11UnorderedAccessView* pVerticesUAV_;


	std::vector<MeshSubset> meshSubsets_;	
};
