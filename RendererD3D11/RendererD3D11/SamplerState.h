#pragma once

class SamplerState final
	: public ISamplerState
{
private:
	friend Renderer;

	SamplerState();

	virtual ~SamplerState();

	bool Init
	(
		ID3D11SamplerState* pLinearClamp,
		ID3D11SamplerState* pLinearWarp,
		ID3D11SamplerState* pAnisotropicClamp,
		ID3D11SamplerState* pAnisotropicWarp
	);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting(uint32_t slot) override;

	void __stdcall ChangeSampler(E_SAMPLER_TYPE samplerType) override;

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pCurSampler_;
	ID3D11SamplerState* pLinearClamp_;
	ID3D11SamplerState* pLinearWarp_;
	ID3D11SamplerState* pAnisotropicClamp_;
	ID3D11SamplerState* pAnisotropicWarp_;
};



enum class E_SAMPLER_TYPE_T
{
	LINEAR_CLAMP = 0,
	LINEAR_WARP,
	ANISOTROPIC_CLAMP,
	ANISOTROPIC_WARP
};


class SamplerStateT final
	: public IUnknown
{
private:
	friend Renderer;

	SamplerStateT();

	virtual ~SamplerStateT();

	bool Init(const E_SAMPLER_TYPE_T& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static SamplerStateT* Create(const E_SAMPLER_TYPE_T& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);

	void BindPS(UINT slot);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pSamplerState_;
};