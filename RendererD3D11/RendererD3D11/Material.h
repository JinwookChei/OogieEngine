#pragma once

class VertexShader;
class PixelShader;
class SamplerState;

class Material final
	: public IMaterial
{
public:
	Material();

	virtual ~Material();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall SetVertexShader(IShader* pVertexShader) override;

	void __stdcall SetPixelShader(IShader* pPixelShader) override;

	void __stdcall SetSampler(ISamplerState* pSamplerState, uint32_t slot = 0) override;

	void __stdcall Setting() override;

private:
	void CleanUp();

	ULONG refCount_;

	VertexShader* pVertexShader_;

	PixelShader* pPixelShader_;

	SamplerState* pSamplerState_;

	uint32_t samplerSlot_;
};