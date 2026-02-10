#pragma once

class DepthState final : public IUnknown
{
private:
	//friend Renderer;
	DepthState();

	virtual ~DepthState();

	bool Init(bool enableDepthTest, bool wirteDepth);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Bind();

	void UnBind();

private:
	void CleanUp();

private:
	ULONG refCount_;

	bool enableDepth_;

	bool writeDepth_;

	ID3D11DepthStencilState* pDepthState_;

public:
	static void InitGlobalDepthStates();
	static void ShutDown();
	static DepthState* Create(bool enableDepthTest, bool wirteDepth);
	static DepthState* GDepthEnableWrite;
	static DepthState* GDepthEnableReadOnly;
	static DepthState* GDepthDisable;
};