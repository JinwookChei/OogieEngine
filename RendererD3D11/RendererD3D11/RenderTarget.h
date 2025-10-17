#pragma once

class Texture;
class RenderTarget final
	: public IRenderTarget {
private:
	friend Renderer;

	RenderTarget();

	virtual ~RenderTarget();

	bool Init(const Color& clearColor, Texture* pRenderTexture, Texture* pDepthTexture);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Clear() override;

	void __stdcall Setting() override;

	void __stdcall BindRenderTextureForPS(uint32_t slot) override;

	void __stdcall ClearRenderTextureForPS(uint32_t slot) override;

	bool SetTexture(Texture* pRenderTexture, Texture* pDepthTexture);

	void __stdcall SetClearColor(const Color& color) override;

private:
	void CleanUp();

	ULONG refCount_;

	Texture* pRenderTexture_;

	Texture* pDepthTexture_;

	D3D11_VIEWPORT viewport_;

	Color clearColor_;
};