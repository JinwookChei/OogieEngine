#pragma once

class Texture;
class RenderTarget final
	: public IRenderTarget {
public:
	RenderTarget();

	virtual ~RenderTarget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool CreateDepthTexture();

	bool SetTexture(Texture* pTexture);

	void Clear();

	void Setting();

private:
	void CleanUp();

	ULONG refCount_;

	Texture* pRenderTexture_;

	Texture* pDepthTexture_;

	D3D11_VIEWPORT viewport_;

	Color clearColor_;
};