#pragma once

enum class E_FILLMODE_TYPE
{
	SOLID = 0,
	WIREFRAME
};

enum class E_RENDER_TECHNIQUE_TYPE
{
	Forward = 0,
	Deferred
};

struct ForwardRenderingDesc
{
	unsigned int fmtColor_ = 2;		//DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	unsigned int fmtDepth_ = 45;	//DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT
	bool useDepthStencil_ = true;
};
 
struct DeferredRenderingDesc
{
	unsigned int fmtAlbedo_ = 28;	//DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	unsigned int fmtNormal_ = 10;	//DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
	unsigned int fmtMaterial_ = 28;
	unsigned int fmtDepth_ = 45;
};

struct RenderTargetDesc
{
	Float2 size_{ 0.0f, 0.0f };
	Color clearColor_{ 0.2f, 0.4f, 0.6f, 1.0f };

	E_RENDER_TECHNIQUE_TYPE renderTechniqueType_ = E_RENDER_TECHNIQUE_TYPE::Forward;

	union
	{
		ForwardRenderingDesc forwardDesc_;
		DeferredRenderingDesc deferredDesc_;
	};

	RenderTargetDesc() = delete;

	RenderTargetDesc(E_RENDER_TECHNIQUE_TYPE renderTechniqueType = E_RENDER_TECHNIQUE_TYPE::Forward)
		: size_({ 0.0f, 0.0f }),
		clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f }), 
		renderTechniqueType_(renderTechniqueType)
	{
		switch (renderTechniqueType_)
		{
		case E_RENDER_TECHNIQUE_TYPE::Forward:
			forwardDesc_.fmtColor_ = 2;
			forwardDesc_.fmtDepth_ = 45;
			forwardDesc_.useDepthStencil_ = true;
			break;
		case E_RENDER_TECHNIQUE_TYPE::Deferred:
			deferredDesc_.fmtAlbedo_ = 28;	//DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			deferredDesc_.fmtNormal_ = 10;	//DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT;
			deferredDesc_.fmtMaterial_ = 28;
			deferredDesc_.fmtDepth_ = 45;
			break;
		default:
			break;
		}
	}
};


struct TextureDesc
{
	Float2 size_;

	UINT colorData_;
};

struct ITexture : public IUnknown
{
	virtual void __stdcall Setting(UINT slot) = 0;
};

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

struct MaterialDesc
{
	bool useTexture = false;
	float shineness = 16.0f;
	Float3 specularColor = { 0.7f,0.7f ,0.7f };
};

struct IMaterial : public IUnknown
{
	virtual float __stdcall GetShineness() const = 0;

	virtual void __stdcall SetShineness(float shineness) = 0;

	virtual const Float3& __stdcall GetSpecularColor() const = 0;

	virtual void __stdcall SetSpecularColor(const Float3& specularColor) = 0;
};


struct IRasterizer : public IUnknown {
	virtual void __stdcall Setting() = 0;

	virtual void __stdcall ChangeFillMode(E_FILLMODE_TYPE fillModeType) = 0;
};

enum class E_RENDER_TEXTURE_TYPE
{
	Albedo = 0,
	Normal,
	Specular,
	Depth,
	MAX,
};

struct IRenderTarget : public IUnknown
{
	virtual void __stdcall Bind() = 0;

	virtual void __stdcall Clear() = 0;

	virtual RenderTargetDesc __stdcall GetDesc() const = 0;

	virtual Float2 __stdcall GetSize() const = 0;

	virtual void __stdcall SetClearColor(const Color& color) = 0;

	virtual void __stdcall BindRenderTexturePS(uint32_t slot) = 0;

	virtual void __stdcall UnBindRenderTexturePS(uint32_t slot) = 0;

	virtual void __stdcall EndRenderPass() = 0;

	virtual void* __stdcall GetShaderResourceView(const E_RENDER_TEXTURE_TYPE& texureType) = 0;
};


enum class E_PARTICLE_PATTERN_TYPE
{
	EXPLOSION = 0,
	FUME
};

struct ParticleDesc
{
	unsigned int maxNum_;

	E_PARTICLE_PATTERN_TYPE patternType_;

	ITexture* pTexture_;
};

struct IParticle : public IUnknown
{

};

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

	virtual void __stdcall RenderParticles(IParticle* pParticle, const Float4x4& viewProj, const Float3& cameraRight, const Float3& cameraUp) = 0;

	virtual void __stdcall DrawDebugLine(const Float3& start, const Float3& end, const Float4& color) = 0;

	virtual void __stdcall DrawDebugRay(const Float3& origin, Float3& dir, float length, const Color& color) = 0;

	virtual void __stdcall RenderDebug() = 0;

	virtual void __stdcall RenderMerge(IRenderTarget* pDepthTarget, IRenderTarget* pSrcTarget) = 0;

	virtual void __stdcall RenderBegin() = 0;

	virtual void __stdcall RenderFinal(IRenderTarget* pSrcTarget) = 0;

	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;

	virtual IMesh* __stdcall CreateMesh(const MeshDesc& desc) = 0;

	virtual IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) = 0;

	virtual IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) = 0;

	virtual IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) = 0;

	virtual IParticle* __stdcall CreateParticle(const ParticleDesc& desc) = 0;

	virtual ITexture* __stdcall LoadTextureFromDirectXTex(const wchar_t* fileName, bool isNormalMap) = 0;

	virtual ITexture* __stdcall CreateTexture(const TextureDesc& desc) = 0;
};

