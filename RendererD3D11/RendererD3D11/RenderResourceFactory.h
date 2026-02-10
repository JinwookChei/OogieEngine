#pragma once

struct RenderResourceFactory : public IRenderResourceFactory
{
public:
	RenderResourceFactory();
	~RenderResourceFactory();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
	ULONG __stdcall AddRef() override;
	ULONG __stdcall Release() override;

	IMesh* __stdcall CreateMesh(const MeshDesc& desc) override;
	//IShader* __stdcall CreateShader(const ShaderDesc& desc) override;
	IMaterial* __stdcall CreateMaterial(const MaterialDesc& desc) override;
	ITexture* __stdcall CreateTextureFromFile(const wchar_t* fileName, bool isNormalMap) override;
	ITexture* __stdcall CreateTexture(const TextureDesc& desc) override;
	ITexture* CreateTexture(const Float2& size, DXGI_FORMAT format, uint32_t flag);
	ITexture* CreateTexture(const D3D11_TEXTURE2D_DESC& desc);
	IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) override;
	IRenderTarget* __stdcall CreateForwardRenderTarget(const RenderTargetDesc& desc);
	IRenderTarget* __stdcall CreateDeferredRenderTarget(const RenderTargetDesc& desc);
	IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) override;
	IParticle* __stdcall CreateParticle(const ParticleDesc& desc) override;
	IPSO* __stdcall CreatePipelineStateObject(const PipelineStateDesc& desc) override;

private:
	ULONG refCount_;
};
