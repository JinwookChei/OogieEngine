#pragma once

class D3D11Texture;
class D3D11RenderTarget final
	: public IRenderTarget {
public:
	D3D11RenderTarget();

	virtual ~D3D11RenderTarget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Clear() override;

	void __stdcall Setting() override;

	void __stdcall BindRenderTextureForPS(uint32_t slot) override;

	void __stdcall ClearRenderTextureForPS(uint32_t slot) override;

	bool SetTexture(D3D11Texture* pRenderTexture, D3D11Texture* pDepthTexture);

	void __stdcall SetClearColor(const Color& color) override;

	//bool CreateDepthTexture();
	// 
	//bool SetDepthTexture(D3D11Texture* pDepthTexture);

private:
	void CleanUp();

	ULONG refCount_;

	D3D11Texture* pRenderTexture_;

	D3D11Texture* pDepthTexture_;

	D3D11_VIEWPORT viewport_;

	Color clearColor_;
};