#pragma once

class Texture;
class RenderTarget final
	:public IRenderTarget {
public:
	RenderTarget();

	virtual ~RenderTarget();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool SetTexture(Texture* texture);

	bool CreateDepthTexture();

	void Clear();

	void Setting();

private:
	void CleanUp();

	ULONG refCount_;

	Texture* renderTexture_;

	Texture* depthTexture_;

	Color clearColor_;

	D3D11_VIEWPORT viewport_;
};