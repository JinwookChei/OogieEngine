#pragma once


struct IRenderResourceFactory : public IUnknown
{
	virtual IMesh* __stdcall CreateMesh(const MeshDesc& desc) = 0;
	virtual IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) = 0;
	virtual ITexture* __stdcall CreateTextureFromFile(const wchar_t* fileName, bool isNormalMap) = 0;
	virtual ITexture* __stdcall CreateTexture(const TextureDesc& desc) = 0;
	virtual IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) = 0;
	virtual IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) = 0;
	virtual IParticle* __stdcall CreateParticle(const ParticleDesc& desc) = 0;
	virtual IPSO* __stdcall CreatePipelineStateObject(const PipelineStateDesc& desc) = 0;
};