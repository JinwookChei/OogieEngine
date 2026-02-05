#pragma once

struct MeshDesc
{
	E_VERTEX_FORMAT vertexFormat;

	uint32_t vertexFormatSize;
	uint32_t vertexCount;
	void* vertices;

	uint32_t indexFormatSize;
	uint32_t indexCount;
	void* indices;
};


struct IMesh : public IUnknown
{
	virtual void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const = 0;

	virtual void __stdcall GetIndicesData(uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutIndices) const = 0;
};