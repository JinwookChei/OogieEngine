#pragma once

class Shader;
class ConstantBuffer;
class SamplerState;
class BlendStateT;


class FinalPass final
	: public IUnknown
{
	friend class Renderer;

	FinalPass();

	~FinalPass();

	bool Init();

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Render(IRenderTarget* pSrcTarget);

private:
	bool InitScreenQuad();

	bool InitShaders();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();

	ULONG refCount_;

	Shader* pShader_;

	Mesh* pScreenQuad_;

	SamplerState* pSamplerState_;
	BlendState* pBlendState_;
};
