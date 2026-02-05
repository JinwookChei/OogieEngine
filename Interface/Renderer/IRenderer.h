#pragma once

struct CameraFrameData
{
	Float4x4 view;
	Float4x4 projection;

	Float2 screenOffset;
	Float2 screenScale;
	Float2 screenResolution;
	Float2 pad0;

	Float4 camPos;
};

struct ObjectRenderData
{
	Float4x4 world;
	IMesh* pMesh;
	IMaterial* pMaterial;
	ITexture* albedo_;
	ITexture* normal_;
};

struct LightRenderData
{
	Color lightDiffuse;
	Color lightSpecular;
	Color lightAmbient;

	Float3 direction_D_S; // Spot, DirectionÀÌ¶û °øÀ¯.
	float range_S_P;
	Float3 position_S_P;

	float exponent_S;
	float innerAngle_S;
	float outerAngle_S;

	float attenuationConst_S_P;
	float attenuationLinear_S_P;
	float attenuationQuad_S_P;

	// 0 -> DirectionLight
	// 1 -> SpotLight
	// 2 -> PointLight
	float lightType;

	Float2 pad;
};

struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	virtual void* __stdcall GetDeviceHandle() = 0;

	virtual void* __stdcall GetDeviceContextHandle() = 0;

	virtual void __stdcall UpdateCameraFrame(const CameraFrameData& cameraFrameData) = 0;

	virtual void __stdcall RenderGBuffer(const ObjectRenderData& objectData) = 0;

	virtual void __stdcall RenderLightBegin(IRenderTarget* pGBufferTarget) = 0;

	virtual void __stdcall RenderLight(const LightRenderData& lightData) = 0;

	virtual void __stdcall RenderLightEnd(IRenderTarget* pGBufferTarget) = 0;

	virtual void __stdcall UpdateParticles(IParticle* pParticle, double deltaTime) = 0;

	virtual void __stdcall RenderParticles(IParticle* pParticle, const Float4x4 worldTransform, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp) = 0;

	virtual void __stdcall DrawDebugLine(const Float3& start, const Float3& end, const Float4& color) = 0;

	virtual void __stdcall DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color) = 0;

	virtual void __stdcall RenderDebug() = 0;

	virtual void __stdcall RenderMerge(IRenderTarget* pSrcTarget) = 0;
	//virtual void __stdcall RenderMerge(IRenderTarget* pDepthTarget, IRenderTarget* pSrcTarget) = 0;

	virtual void __stdcall RenderBegin() = 0;

	virtual void __stdcall RenderFinal(IRenderTarget* pSrcTarget) = 0;

	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;

	/*virtual IMesh* __stdcall CreateMesh(const MeshDesc& desc) = 0;

	virtual IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) = 0;

	virtual IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) = 0;

	virtual IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) = 0;

	virtual IParticle* __stdcall CreateParticle(const ParticleDesc& desc) = 0;

	virtual ITexture* __stdcall LoadTextureFromDirectXTex(const wchar_t* fileName, bool isNormalMap) = 0;

	virtual ITexture* __stdcall CreateTexture(const TextureDesc& desc) = 0;*/
};

