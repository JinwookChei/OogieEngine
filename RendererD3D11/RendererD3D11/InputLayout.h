#pragma once

class D3D11InputLayout final
	: public IInputLayout
{
public:
	D3D11InputLayout();

	virtual ~D3D11InputLayout();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

	bool Create(IVertex* pVertex, IShader* pVertexShader);

private:
	void CleanUp();

	UINT refCount_;

	ID3D11InputLayout* pInputLayout_;
};
