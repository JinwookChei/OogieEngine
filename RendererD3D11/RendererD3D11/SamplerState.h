#pragma once

class SamplerState final : public IUnknown
{
private:
	//friend Renderer;
	SamplerState();

	virtual ~SamplerState();

	bool Init(const E_SAMPLER_PRESET& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void BindPS(UINT slot);

private:
	void CleanUp();

private:
	ULONG refCount_;
	E_SAMPLER_PRESET samplerType_;
	float minLOD_;
	float maxLOD_;
	unsigned int maxAnisotropy_;
	ID3D11SamplerState* pSamplerState_;

public:
	static void InitGlobalSamplers();
	static void ShutDown();
	static SamplerState* Create(const E_SAMPLER_PRESET& samplerType, float minLOD, float maxLOD, unsigned int maxAnisotropy);
	static SamplerState* GSamplerLinearClamp;
	static SamplerState* GSamplerLinearWarp;
	static SamplerState* GSamplerAnisotropicClamp;
	static SamplerState* GSamplerAnisotropicWarp;
};