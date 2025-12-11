#pragma once

enum class E_SAMPLER_TYPE
{
	LINEAR_CLAMP = 0,
	LINEAR_WARP,
	ANISOTROPIC_CLAMP,
	ANISOTROPIC_WARP
};


class SamplerState final
	: public IUnknown
{
private:
	friend Renderer;

	SamplerState();

	virtual ~SamplerState();

	bool Init(const E_SAMPLER_TYPE& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static SamplerState* Create(const E_SAMPLER_TYPE& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);

	void BindPS(UINT slot);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pSamplerState_;
};