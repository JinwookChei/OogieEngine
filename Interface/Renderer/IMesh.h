#pragma once

enum class E_MESH_PRIMITIVE_TYPE
{
	Point = 0,
	Line,
	Triangle
};

enum class E_MESH_RESOURCE_FLAG : uint32_t
{
	None = 0,
	ShaderResource = 1 << 0,
	UnorderedAccess = 1 << 1
};

inline E_MESH_RESOURCE_FLAG operator|(E_MESH_RESOURCE_FLAG lhs, E_MESH_RESOURCE_FLAG rhs)
{
	return static_cast<E_MESH_RESOURCE_FLAG>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline E_MESH_RESOURCE_FLAG operator&(E_MESH_RESOURCE_FLAG lhs, E_MESH_RESOURCE_FLAG rhs)
{
	return static_cast<E_MESH_RESOURCE_FLAG>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

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

	E_MESH_PRIMITIVE_TYPE primitiveType;
	E_MESH_RESOURCE_FLAG resourceFlag;

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