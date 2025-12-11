#pragma once

class Shader;
class ConstantBuffer;
class SamplerStateT;
class BlendStateT;


class MergePass final
	: public IUnknown
{
	friend class Renderer;

	MergePass();

	~MergePass();

	bool Init();

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Render(IRenderTarget* pDepthTarget, IRenderTarget* pSrcTarget);

private:
	bool InitScreenQuad();

	bool InitShaders();

	bool InitBlendState();

	bool InitSamplerState();

	void CleanUp();

	ULONG refCount_;

	Shader* pShader_;

	Mesh* pScreenQuad_;

	SamplerStateT* pSamplerState_;
	BlendStateT* pBlendState_;
};
