#pragma once


enum class E_DEPTH_PRESET
{
	DEPTH_ENABLE_WRITE = 0,
	DEPTH_ENABLE_READ_ONLY,
	DEPTH_DISABLE
};

// 이건 아직 임시로 적용.
enum class E_RASTERIZER_PRESET
{
	SOLID = 0,
	WIRE_FRAME,
	DISABLE
};

struct PipelineStateDesc
{
	//IMesh* pMesh;
	//IMaterial* pMaterial;

	uint16_t meshSlotCount;
	uint16_t materialSlotCount;

	E_DEPTH_PRESET depthState;
	E_RASTERIZER_PRESET rasterizerMode;
};

// Pipeline State Object
struct IPSO : IUnknown
{
	virtual IMesh* __stdcall GetMesh(uint16_t slot) const = 0;

	virtual IMaterial* __stdcall GetMaterial(uint16_t slot) const = 0;

	virtual void __stdcall SetMeshToSlot(uint16_t slot, IMesh* pMesh) = 0;

	virtual void __stdcall SetMaterialToSlot(uint16_t slot, IMaterial* pMaterial) = 0;
};