#pragma once

enum class E_SHADER_TYPE
{
	Vertex = 0,
	Pixel
};

enum class E_FILLMODE_TYPE
{
	WireFrame = 0,
	Solid
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

struct ITexture : public IUnknown
{
	virtual void __stdcall Setting(UINT slot) = 0;

	//virtual ID3D11RenderTargetView* __stdcall RenderTargetView() const = 0;

	//virtual ID3D11DepthStencilView* __stdcall DepthStencilView() const = 0;

	//virtual ID3D11ShaderResourceView* __stdcall ShaderResourceView() const = 0;
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
	virtual void __stdcall Setting() = 0;

	virtual bool __stdcall AddInputLayout(const char* pSemanticName, uint32_t semanticIndex, uint32_t format, uint32_t inputSlot, bool isInstanceData) = 0;

	virtual bool __stdcall Draw() = 0;

	virtual void __stdcall GetVerticesData(E_VERTEX_FORMAT* pOutFormat, uint32_t* pOutStride, uint32_t* pOutCount, void** ppOutVertices) const = 0;
};

struct MaterialDesc
{
	const wchar_t* VS = nullptr;
	const wchar_t* PS = nullptr;
	bool useTexture = false;
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

	virtual void __stdcall SetFillMode(E_FILLMODE_TYPE fillModeType) = 0;
};

struct IBlendState : public IUnknown
{
	virtual void __stdcall Clear() = 0;

	virtual void __stdcall Setting() = 0;
};

enum class E_VIEW_TYPE
{
	RenderTargetView =0,
	DepthStencilView,
	ShaderResourceView
};

struct ViewDesc
{
	E_VIEW_TYPE viewType;
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
	virtual void __stdcall Clear() = 0;

	virtual void __stdcall Setting() = 0;

	virtual RenderTargetDesc __stdcall GetDesc() const = 0;

	virtual Float2 __stdcall GetSize() const = 0;

	virtual void __stdcall SetClearColor(const Color& color) = 0;

	virtual void __stdcall BindRenderTextureForPS(uint32_t slot) = 0;

	virtual void __stdcall ClearRenderTextureForPS(uint32_t slot) = 0;

	virtual void __stdcall EndRenderPass() = 0;

	virtual void* __stdcall GetShaderResourceView(const E_RENDER_TEXTURE_TYPE& texureType) = 0;
};


struct IRenderer : public IUnknown {
	virtual bool __stdcall Initialize(void* hWnd, uint32_t width, uint32_t height) = 0;

	virtual void* __stdcall GetDeviceHandle() = 0;

	virtual void* __stdcall GetDeviceContextHandle() = 0;

	virtual void __stdcall RenderBegin() = 0;

	virtual void __stdcall RenderEnd() = 0;

	virtual uint64_t __stdcall DrawCallCount() = 0;

	virtual IInputLayout* __stdcall CreateLayout(IMesh* pVertex, IShader* pVertexShader) = 0;

	//virtual IMesh* __stdcall CreateMesh(void* pVertices, uint32_t vertexSize, uint32_t vertexCount, void* pIndices = nullptr, uint32_t indexTypeSize = 0, uint32_t indexCount = 0) = 0;
	virtual IMesh* __stdcall CreateMesh(const MeshDesc& desc) = 0;

	virtual IMaterial* __stdcall CreateMaterial(const MaterialDesc& materialDesc) = 0;

	virtual IConstantBuffer* __stdcall CreateConstantBuffer(uint32_t bufferSize) = 0;

	virtual IRasterizer* __stdcall  CreateRasterizer(bool frontCounterClockwise, bool backFaceCulling) = 0;

	virtual IRenderTarget* __stdcall CreateRenderTarget(const RenderTargetDesc& desc) = 0;

	virtual IBlendState* __stdcall CreateBlendState(uint32_t srcBlend, uint32_t destBlend, uint32_t srcBlendAlpha, uint32_t destBlendAlpha, float blendFactor[4] = nullptr) = 0;

	virtual ITexture* __stdcall LoadTextureFromDirectXTex(const wchar_t* fileName, bool isNormalMap) = 0;
};