#pragma once

enum class EShaderType
{
	Vertex = 0,
	Pixel
};

enum class EFillModeType
{
	WireFrame = 0,
	Solid
};



enum class ERenderTechniqueType
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

	ERenderTechniqueType renderTechniqueType_ = ERenderTechniqueType::Forward;

	union
	{
		ForwardRenderingDesc forwardDesc_;
		DeferredRenderingDesc deferredDesc_;
	};

	RenderTargetDesc() = delete;

	RenderTargetDesc(ERenderTechniqueType renderTechniqueType = ERenderTechniqueType::Forward)
		: size_({ 0.0f, 0.0f }),
		clearColor_({ 0.2f, 0.4f, 0.6f, 1.0f }), 
		renderTechniqueType_(renderTechniqueType)
	{
		switch (renderTechniqueType_)
		{
		case ERenderTechniqueType::Forward:
			forwardDesc_.fmtColor_ = 2;
			forwardDesc_.fmtDepth_ = 45;
			forwardDesc_.useDepthStencil_ = true;
			break;
		case ERenderTechniqueType::Deferred:
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

struct ITexture : public IUnknown
{
};


struct IShader : public IUnknown
{
	//virtual void __stdcall Setting() = 0;
};


struct ISamplerState : public IUnknown
{
	//virtual void __stdcall Setting(uint32_t slot) = 0;
};


struct IInputLayout : public IUnknown {
	virtual void __stdcall Setting() = 0;
};

struct IMesh : public IUnknown
{
	virtual void __stdcall Setting() = 0;

	virtual bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) = 0;

	virtual bool __stdcall Draw() = 0;
};

struct MaterialDesc
{
	const wchar_t* VS = nullptr;
	const wchar_t* PS = nullptr;
	bool samplerLinear = false;
	bool samplerClamp = false;
	float shineness = 16.0f;
	Float3 specularColor = { 0.7f,0.7f ,0.7f };
};

struct IMaterial : public IUnknown
{
	virtual void __stdcall Setting() = 0;

	virtual IShader* __stdcall GetVertexShader() = 0;

	virtual float __stdcall GetShineness() const = 0;

	virtual void __stdcall SetShineness(float shineness) = 0;

	virtual const Float3& __stdcall GetSpecularColor() const = 0;

	virtual void __stdcall SetSpecularColor(const Float3& specularColor) = 0;
};

struct IConstantBuffer : public IUnknown {
	virtual void __stdcall Update(void* pSrcData) = 0;

	virtual void __stdcall VSSetting(uint32_t slot) = 0;

	virtual void __stdcall PSSetting(uint32_t slot) = 0;
};


struct IRasterizer : public IUnknown {
	virtual void __stdcall Setting() = 0;

	virtual void __stdcall SetFillMode(EFillModeType fillModeType) = 0;
};

struct IBlendState : public IUnknown
{
	virtual void __stdcall Clear() = 0;

	virtual void __stdcall Setting() = 0;
};

struct IRenderTarget : public IUnknown
{
	virtual void __stdcall Clear() = 0;

	virtual void __stdcall Setting() = 0;

	virtual RenderTargetDesc __stdcall GetDesc() const = 0;

	virtual Float2 __stdcall GetSize() const = 0;

	virtual void __stdcall SetClearColor(const Color& color) = 0;

	virtual void __stdcall BindRenderTextureForPS(uint32_t slot) = 0;

	virtual void __stdcall ClearRenderTextureForPS(uint32_t slot) = 0;

	virtual void __stdcall EndRenderPass() = 0;
};



struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	virtual void __stdcall RenderBegin() = 0;

	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;

	virtual IInputLayout* __stdcall CreateLayout(IMesh* pVertex, IShader* pVertexShader) = 0;

	virtual IMesh* __stdcall CreateMesh(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) = 0;

	//virtual IMaterial* __stdcall CreateMaterial(const wchar_t* VS, const wchar_t* PS, bool samplerLinear, bool samplerClamp) = 0;
	virtual IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) = 0;

	virtual IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) = 0;

	virtual IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) = 0;

	virtual IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) = 0;

	virtual IBlendState* __stdcall CreateBlendState(uint32_t srcBlend, uint32_t destBlend, uint32_t srcBlendAlpha, uint32_t destBlendAlpha, float blendFactor[4] = nullptr) = 0;
};