#pragma once

class Texture;
class RenderTarget final
	: public IRenderTarget {
private:
	friend Renderer;

	RenderTarget();

	virtual ~RenderTarget();

	bool Init(const RenderTargetDesc& desc, Texture* pRenderTexture, Texture* pDepthTexture);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Clear() override;

	void __stdcall Setting() override;

	RenderTargetDesc __stdcall GetDesc() const override;

	Float2 __stdcall GetSize() const override;

	void __stdcall SetClearColor(const Color& color) override;

	void __stdcall BindRenderTextureForPS(uint32_t slot) override;

	void __stdcall ClearRenderTextureForPS(uint32_t slot) override;

	bool SetTexture(Texture* pRenderTexture, Texture* pDepthTexture);

private:
	void CleanUp();

	ULONG refCount_;

	// Desc
	Color clearColor_;
	unsigned int fmtColor_;
	unsigned int fmtDepth_;
	bool useDepthStencil_;
	//

	Texture* pRenderTexture_;

	Texture* pDepthTexture_;

	D3D11_VIEWPORT viewport_;
};