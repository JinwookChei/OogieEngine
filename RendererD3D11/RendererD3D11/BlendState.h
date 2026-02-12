#pragma once

class BlendState : public IUnknown
{
private:

	BlendState();
	virtual ~BlendState();

	bool Init(const E_BLEND_PRESET& blendPreset);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Bind();

	void UnBind();

private:
	void CleanUp();

private:
	ULONG refCount_;

	float blendFactor_[4];
	ID3D11BlendState* pBlendState_;

public:
	static void InitGlobalBlendStates();
	static void ShutDown();
	static BlendState* Create(const E_BLEND_PRESET& blendPreset);
	static BlendState* GOpaqueBlend;
	static BlendState* GAlphaBlend;
	static BlendState* GAdditiveBlend;
};