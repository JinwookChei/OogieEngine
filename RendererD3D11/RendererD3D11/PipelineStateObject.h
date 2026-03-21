#pragma once

class Mesh;
class Material;
class Rasterizer;
class DepthState;

class PipelineStateObject : public IPSO
{
public:
	PipelineStateObject();
	~PipelineStateObject();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

	IMesh* __stdcall GetMesh(uint16_t slot) const override;
	uint32_t __stdcall GetMeshSlotCount() const override;
	void __stdcall SetMeshToSlot(uint16_t slot, IMesh* pMesh);

	IMaterial* __stdcall GetMaterial(uint16_t slot) const override;
	uint32_t __stdcall GetMaterialSlotCount() const override;
	void __stdcall SetMaterialToSlot(uint16_t slot, IMaterial* pMaterial);
	

	//uint32_t GetMeshSlotCount() const;

	//uint32_t GetMaterialSlotCount() const;

	const E_DEPTH_PRESET& GetDepthState() const;

	const E_RASTERIZER_PRESET& GetRasterizerMode() const;

private:
	void CleanUp();

private:
	ULONG refCount_;

	uint32_t meshSlotCount_;
	Mesh** ppMesheSlot_;

	uint32_t materialSlotCount_;
	Material** ppMaterialSlot_;

	E_DEPTH_PRESET depthState_;
	E_RASTERIZER_PRESET rasterizerMode_;

public:
	static PipelineStateObject* Create(const PipelineStateDesc& desc);
};
