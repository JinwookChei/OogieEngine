#pragma once

class InputLayout final
	: public IInputLayout
{
private:
	friend Renderer;

	InputLayout();

	virtual ~InputLayout();

	bool Init(ID3D11InputLayout* pInputLayout);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

private:
	void CleanUp();

	UINT refCount_;

	ID3D11InputLayout* pInputLayout_;
};
