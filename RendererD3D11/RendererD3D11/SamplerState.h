#pragma once

class D3D11SamplerState final
	: public ISamplerState
{
public:
	D3D11SamplerState();

	virtual ~D3D11SamplerState();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting(uint32_t slot) override;

	bool CreateSampler(bool linear, bool clamp);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pSamplerState_;
};