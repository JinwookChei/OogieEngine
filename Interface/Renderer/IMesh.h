#pragma once

enum class E_MESH_PRIMITIVE_TYPE
{
	POINT = 0,
	LINE,
	TRIANGLE
};

enum class E_MESH_USAGE
{
	DEFAULT,   // GPU read/write
	IMMUTABLE, // GPU read only (init data required)
	DYNAMIC,   // CPU write
	STAGING    // CPU read/write
};

enum class E_MESH_BIND_FLAG : uint32_t
{
	NONE = 0,
	VERTEX_BUFFER = 1 << 0,
	INDEX_BUFFER = 1 << 1,
	CONSTANT_BUFFER = 1 << 2,
	SHADER_RESOURCE = 1 << 3,
	UNORDERED_ACCESS = 1 << 4
};

inline E_MESH_BIND_FLAG operator|(E_MESH_BIND_FLAG lhs, E_MESH_BIND_FLAG rhs)
{
	return static_cast<E_MESH_BIND_FLAG>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline E_MESH_BIND_FLAG operator&(E_MESH_BIND_FLAG lhs, E_MESH_BIND_FLAG rhs)
{
	return static_cast<E_MESH_BIND_FLAG>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
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
	E_MESH_USAGE usage;
	E_MESH_BIND_FLAG bindFlag;

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