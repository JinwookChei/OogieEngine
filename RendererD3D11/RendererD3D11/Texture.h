#pragma once

class Texture final :
	public ITexture {
public:
	Texture();

	virtual ~Texture();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static Texture* Create(const Float2& size, DXGI_FORMAT format, uint32_t flag);

	static Texture* Create(const D3D11_TEXTURE2D_DESC& desc);

	void BindRenderTextureForPS(uint32_t slot);

	void ClearRenderTextureForPS(uint32_t slot);

	bool SetTexture(ID3D11Texture2D* pTexture);

	Float2 Size() const;

	ID3D11RenderTargetView* RenderTargetView() const;

	ID3D11DepthStencilView* DepthStencilView() const;

	ID3D11ShaderResourceView* ShaderResourceView() const;

private:
	bool CreateRenderTargetView();

	bool CreateDepthStencilView();

	bool CreateShaderResourceView();

	void CleanUp();

	ULONG refCount_;

	ID3D11Texture2D* pTexture_;

	ID3D11RenderTargetView* pRenderTargetView_;

	ID3D11DepthStencilView* pDepthStencilView_;

	ID3D11ShaderResourceView* pShaderResourceView_;

	D3D11_TEXTURE2D_DESC desc_;
};