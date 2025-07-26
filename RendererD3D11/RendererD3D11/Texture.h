#pragma once

class Texture final :
	public ITexture {
public:
	Texture();

	virtual ~Texture();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	float Width() const;

	float Height() const;

	bool SetTexture(ID3D11Texture2D* texture);

	bool CreateRenderTargetView();

	ID3D11RenderTargetView* RenderTargetView();

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11Texture2D* texture_;

	ID3D11RenderTargetView* renderTargetView_;

	D3D11_TEXTURE2D_DESC desc_;
};