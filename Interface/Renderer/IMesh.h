#pragma once


struct MeshDesc
{
	struct MeshSubsetDesc
	{
		uint32_t materialSlot;
		uint16_t indexFormatSize;
		size_t  indexCount;
		void* pIndices;

		MeshSubsetDesc
		(
			uint32_t materialSlot,
			uint16_t indexFormatSize,
			size_t indexCount,
			void* pIndices
		)
			: materialSlot(materialSlot)
			, indexFormatSize(indexFormatSize)
			, indexCount(indexCount)
			, pIndices(pIndices)
		{
		};
	};

	//E_VERTEX_FORMAT vertexFormat;
	uint16_t vertexFormatSize;
	size_t vertexCount;
	void* pVertices;

	std::vector<MeshSubsetDesc> meshSubsets;
};


struct IMesh : public IUnknown
{
	//virtual void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const = 0;

	//virtual void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const = 0;
};