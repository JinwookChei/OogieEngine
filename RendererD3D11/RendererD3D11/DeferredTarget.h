#pragma once

const int RENDER_BUFFER_COUNT = 4;

class DeferredTarget final : public IRenderTarget
{
	friend class RenderResourceFactory;

	DeferredTarget();
	virtual ~DeferredTarget();
	bool Init
	(
		const RenderTargetDesc& desc,
		Texture* pRenderTextureAlbedo, 
		Texture* pRenderTextureNormal, 
		Texture* pRenderTextureSpecular,
		Texture* pRenderTexturePosition,
		Texture* pDepthTexture
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Bind() override;

	void __stdcall Bind(ITexture* pDepthTexture) override;

	void __stdcall UnBind() override;

	void __stdcall Clear() override;

	RenderTargetDesc __stdcall GetDesc() const override;

	Float2 __stdcall GetSize() const override;

	void __stdcall SetClearColor(const Color& color) override;

	void* __stdcall GetShaderResourceView(const E_RENDER_TEXTURE_TYPE& texureType) override;

	ITexture* __stdcall GetRenderTexture(const E_RENDER_TEXTURE_TYPE& textureType) override;

private:
	bool SetTexture(Texture* pRenderTextureAlbedo, Texture* pRenderTextureNormal, Texture* pRenderTextureSpecular, Texture* pRenderTexturePosition, Texture* pDepthTexture);

	void ClearRenderTexture(Texture* pRenderTexture);

	void ClearDepthTexture(Texture* pDepthTexture);

	void CleanUp();

private:
	ULONG refCount_;
	D3D11_VIEWPORT viewport_;

	Color clearColor_;
	DeferredRenderingDesc desc_;

	Texture* pRenderTextureAlbedo_;
	Texture* pRenderTextureNormal_;
	Texture* pRenderTextureSpecular_;
	Texture* pRenderTexturePosition_;
	Texture* pRenderTextureDepth_;

	ID3D11RenderTargetView* pRTVs_[RENDER_BUFFER_COUNT];
	ID3D11DepthStencilView* pDSV_;
};
