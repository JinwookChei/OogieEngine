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
	struct SubMeshDesc
	{
		uint32_t materialSlot;
		uint32_t indexCount;
		void* pIndices;

		SubMeshDesc
		(
			uint32_t materialSlot,
			uint32_t indexCount,
			void* pIndices
		)
			: materialSlot(materialSlot)
			, indexCount(indexCount)
			, pIndices(pIndices)
		{
		};
	};

	E_VERTEX_TYPE vertexType; 
	E_MESH_PRIMITIVE_TYPE primitiveType;
	E_MESH_USAGE usage;
	E_MESH_BIND_FLAG bindFlag;
	uint32_t bufferSize;
	uint32_t vertexFormatSize;
	uint32_t vertexCount;
	void* pVertices;

	std::vector<SubMeshDesc> subMeshDesc;
};


struct IMesh : public IUnknown
{
	virtual void __stdcall WriteBuffer(const void* data, uint32_t size) = 0;

	virtual AABB __stdcall GetAABB() = 0;

	virtual void __stdcall GetVerticesData(void** ppOutVertices, E_VERTEX_TYPE* pOutType) const = 0;

	virtual void __stdcall GetIndiciesData(void** ppOutIndices, uint32_t* pOutIndicesCount, uint32_t subMeshIndex) const = 0;

	virtual uint32_t __stdcall GetSubMeshCount() const = 0;

	//virtual void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const = 0;

	//virtual void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const = 0;
};