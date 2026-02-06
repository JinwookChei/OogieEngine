#pragma once
class Mesh;
class Material;
class BlendState;
class Rasterizer;
//class SamplerState; -> ÀÌ°Ç Material
class DepthState;


class PipelineStateObject : public IPSO
{
public:
	PipelineStateObject();
	~PipelineStateObject();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

private:
	void CleanUp();

private:
	ULONG refCount_;
	IMesh* pMesh_;
	IMaterial* pMaterial_;
	E_SAMPLER_PRESET samplerState_;
	E_DEPTH_PRESET depthState_;
	E_BLEND_PRESET blendState_;
	E_RASTERIZER_PRESET rasterizerMode_;

public:
	static PipelineStateObject* Create(const PipelineStateDesc& desc);
};
