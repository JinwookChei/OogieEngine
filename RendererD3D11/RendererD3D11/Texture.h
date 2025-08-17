#pragma once

class Texture final :
	public ITexture {
public:
	Texture();

	virtual ~Texture();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static Texture* Create(const D3D11_TEXTURE2D_DESC& desc);

	bool SetTexture(ID3D11Texture2D* texture);

	float Width() const;

	float Height() const;

	ID3D11RenderTargetView* RenderTargetView() const;

	ID3D11DepthStencilView* DepthStencilView() const;

private:
	bool CreateRenderTargetView();

	bool CreateDepthStencilView();

	void Cleanup();

	ULONG refCount_;

	ID3D11Texture2D* texture_;

	ID3D11RenderTargetView* renderTargetView_;

	ID3D11DepthStencilView* depthStencilView_;

	D3D11_TEXTURE2D_DESC desc_;
};