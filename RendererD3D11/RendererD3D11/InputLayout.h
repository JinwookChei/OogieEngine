#pragma once

class InputLayout
	: public IInputLayout
{
public:
	InputLayout();

	virtual ~InputLayout();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

private:
	UINT refCount_;
};
