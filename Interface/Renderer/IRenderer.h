#pragma once

struct CameraFrameData
{
	Float4x4 view;
	Float4x4 projection;

	//Float4x4 inverseView;
	//Float4x4 inverseProjection;

	Float2 screenOffset;
	Float2 screenScale;
	Float2 screenResolution;
	Float2 pad0;

	Float4 camPos;
};

struct ObjectFrameData
{
	Float4x4 worldMatrix;
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

	Float3 direction_D_S; // Spot, Direction이랑 공유.
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

// 임시. -> Const-> Struct buffer로 바꿔야함.
struct AnimConstantBuffer
{
	Float4x4 animTransform[200];
};


struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	virtual void* __stdcall GetDeviceHandle() = 0;

	virtual void* __stdcall GetDeviceContextHandle() = 0;

	virtual void __stdcall UpdateCameraFrame(const CameraFrameData& cameraFrameData) = 0;

	virtual void __stdcall UpdateObjectFrame(const ObjectFrameData& objectFrameData) = 0;

	virtual void __stdcall UpdateLightFrame(const LightRenderData& lightFrameData) = 0;

	virtual void __stdcall UpdateComputeParticleFrame(const ComputeParticleData& computeParticleData) = 0;

	virtual void __stdcall UpdateRenderParticleFrame(const RenderParticleData& renderParticleData) = 0;

	virtual void __stdcall UpdateAnimationFrame(const AnimConstantBuffer& animFrameData) = 0;
	 
	virtual void __stdcall Render(IPSO* pipelineStateObject) = 0;
	virtual void __stdcall RenderParticle_Test(IPSO* pipelineStateObject) = 0;
	virtual void __stdcall Compute(IPSO* pipelineStateObject, UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ) = 0;

	virtual void __stdcall UnBindSRVs(bool bVS, bool bPS) = 0;

	virtual void __stdcall RenderMerge(IRenderTarget* pSrcTarget) = 0;
	
	virtual void __stdcall RenderBegin() = 0;

	virtual void __stdcall RenderFinal(IRenderTarget* pSrcTarget) = 0;

	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;
};

