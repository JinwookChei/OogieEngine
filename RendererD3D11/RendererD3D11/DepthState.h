#pragma once

class DepthState final
	: public IUnknown
{
private:
	friend Renderer;

	DepthState();

	virtual ~DepthState();

	bool Init(bool useDepthTest, bool wirteDepth);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static DepthState* Create(bool useDepthTest, bool wirteDepth);

	void Bind();

	void UnBind();

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11DepthStencilState* pDepthState_;
};