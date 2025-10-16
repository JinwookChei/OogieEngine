#pragma once

class SamplerState final
	: public ISamplerState
{
public:
	SamplerState() = delete;

	SamplerState(ID3D11SamplerState* pSamplerState);

	virtual ~SamplerState();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting(uint32_t slot) override;

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pSamplerState_;
};