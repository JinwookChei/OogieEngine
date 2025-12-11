#pragma once


class BlendState
	: public IBlendState
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


enum class E_BLEND_MODE
{
	OPAQUE_BLEND = 0,				// 블렌딩 없음 (불투명)
	ALPHA_BLEND,					// 일반 알파 블렌딩
	ADDITIVE_BLEND,					// Additive(가산) 블렌딩
};

class BlendStateT : public IUnknown
{
private:

	BlendStateT();
	virtual ~BlendStateT();

	bool Init(const E_BLEND_MODE& blendMode);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static BlendStateT* Create(const E_BLEND_MODE& blendMode);

	void Bind();

	void UnBind();

private:
	void CleanUp();

	ULONG refCount_;

	float blendFactor_[4];
	ID3D11BlendState* pBlendState_;
};