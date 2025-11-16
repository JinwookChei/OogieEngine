#pragma once

class SamplerState final
	: public ISamplerState
{
private:
	friend Renderer;

	SamplerState();

	virtual ~SamplerState();

	bool Init(ID3D11SamplerState* pSamplerState);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting(uint32_t slot) override;

	//void Setting(uint32_t slot);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11SamplerState* pSamplerState_;
};