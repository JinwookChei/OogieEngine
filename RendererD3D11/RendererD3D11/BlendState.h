#pragma once


class BlendState
	: IBlendState
{
private:
	friend Renderer;

	BlendState();

	virtual ~BlendState();

	bool Init
	(
		ID3D11BlendState* pBlendOpaque,
		ID3D11BlendState* pBlendAlpha,
		ID3D11BlendState* pBlendAdditive,
		float blendFactor[4]
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Clear() override;

	void __stdcall Setting() override;

	void __stdcall ChangeBlendMode(const E_BLEND_MODE_TYPE& blendType) override;

private:
	void CleanUp();

	int refCount_;

	ID3D11BlendState* pCurBlendMode_;
	ID3D11BlendState* pBlendOpaque_;
	ID3D11BlendState* pBlendAlpha_;
	ID3D11BlendState* pBlendAdditive_;

	//Float[4]
	float* blendFactor_;
};
