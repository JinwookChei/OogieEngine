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

	E_VERTEX_FORMAT vertexFormat;
	uint16_t vertexFormatSize;
	size_t vertexCount;
	void* pVertices;

	//uint16_t meshSubsetCount;
	std::vector<MeshSubsetDesc> meshSubsets;
};


//struct MeshSubsetDesc
//{
//	uint32_t materialBindSlot_;
//	uint32_t indexStride_;
//	uint32_t indexCount_;
//	void* pIndices_;
//
//	MeshSubsetDesc
//	(
//		uint32_t materialBindSlot,
//		uint32_t indexStride,
//		uint32_t indexCount,
//		void* pIndices
//	)
//		: materialBindSlot_(materialBindSlot)
//		, indexStride_(indexStride)
//		, indexCount_(indexCount)
//		, pIndices_(pIndices)
//	{
//	}
//};
//
//struct MeshDesc
//{
//	E_VERTEX_FORMAT vertexFormat;
//
//	uint32_t vertexFormatSize;
//	uint32_t vertexCount;
//	void* vertices;
//
//	uint8_t meshSubsetCount;
//	std::vector<MeshSubsetDesc> meshSubSets;
//
//	//uint32_t indexFormatSize;
//	//uint32_t indexCount;
//	//void* indices;
//};


struct IMesh : public IUnknown
{
	virtual void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const = 0;

	//virtual void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const = 0;
};