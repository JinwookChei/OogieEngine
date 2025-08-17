#pragma once

class VertexShader;
class PixelShader;

class Material final
	: public IMaterial
{
public:
	Material();

	virtual ~Material();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall SetVertexShader(IShader* vertexShader) override;

	void __stdcall SetPixelShader(IShader* pixelShader) override;

	void __stdcall SetSampler(ISamplerState* sampler, uint32_t slot = 0) override;

	void __stdcall Setting() override;

private:
	void Cleanup();

	ULONG refCount_;

	VertexShader* vertexShader_;

	PixelShader* pixelShader_;

	ISamplerState* sampler_;

	uint32_t samplerSlot_;
};