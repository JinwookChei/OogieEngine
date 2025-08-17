#pragma once

class InputLayout final
	: public IInputLayout
{
public:
	InputLayout();

	virtual ~InputLayout();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool Create(IVertex* vertex, IShader* vertexShader);

	void __stdcall Setting() override;

private:
	void Cleanup();

	UINT refCount_;

	ID3D11InputLayout* inputLayout_;
};
