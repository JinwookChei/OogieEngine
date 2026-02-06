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

	Mesh* pMesh_;
	Material* pMaterial_;
	DepthState* pDepthState_;
	Rasterizer* pRasterizer_;
};
