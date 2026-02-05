#pragma once

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

	virtual void __stdcall Bind(ITexture* pDepthTexture) = 0;

	virtual void __stdcall Clear() = 0;

	virtual RenderTargetDesc __stdcall GetDesc() const = 0;

	virtual Float2 __stdcall GetSize() const = 0;

	virtual void __stdcall SetClearColor(const Color& color) = 0;

	virtual void __stdcall BindRenderTexturePS(uint32_t slot) = 0;

	virtual void __stdcall UnBindRenderTexturePS(uint32_t slot) = 0;

	virtual void __stdcall EndRenderPass() = 0;

	virtual void* __stdcall GetShaderResourceView(const E_RENDER_TEXTURE_TYPE& texureType) = 0;

	virtual ITexture* __stdcall GetDepthTexture() = 0;
};
