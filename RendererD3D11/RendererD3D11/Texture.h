#pragma once

class Texture final : public ITexture 
{
private:
	friend class RenderResourceFactory;
	friend class Renderer;

	Texture();
	virtual ~Texture();
	bool Init(ID3D11Texture2D* pTexture);
	bool Init(ID3D11Texture2D* pTexture, ID3D11ShaderResourceView* pShaderResourceView);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting(UINT slot) override;

	ID3D11RenderTargetView* RenderTargetView() const;

	ID3D11DepthStencilView* DepthStencilView() const;

	ID3D11ShaderResourceView* ShaderResourceView() const;

	void BindRenderTextureForPS(uint32_t slot);

	void ClearRenderTextureForPS(uint32_t slot);

	bool SetTexture(ID3D11Texture2D* pTexture);

	Float2 Size() const;

private:
	bool InitTexture(ID3D11Texture2D* pTexture);

	bool CreateRenderTargetView();

	bool CreateDepthStencilView();

	bool CreateShaderResourceView();

	void CleanUp();

	ULONG refCount_;

	ID3D11Texture2D* pTexture_;

	ID3D11RenderTargetView* pRenderTargetView_;

	ID3D11DepthStencilView* pDepthStencilView_;

	ID3D11ShaderResourceView* pShaderResourceView_;
};