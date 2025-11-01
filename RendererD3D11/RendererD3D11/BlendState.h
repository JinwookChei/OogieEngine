#pragma once


class BlendState
	: IBlendState
{
private:
	friend Renderer;

	BlendState();

	virtual ~BlendState();

	bool Init(ID3D11BlendState* pBlendState, float blendFactor[4]);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Clear() override;

	void __stdcall Setting() override;

private:
	void CleanUp();

	int refCount_;

	ID3D11BlendState* pBlendState_;

	//Float[4]
	float* blendFactor_;
};
