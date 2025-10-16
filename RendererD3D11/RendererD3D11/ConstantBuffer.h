#pragma once

class ConstantBuffer final
	: public IConstantBuffer
{
public:
	ConstantBuffer() = delete;

	ConstantBuffer(ID3D11Buffer* pBuffer);

	virtual ~ConstantBuffer();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Update(void* pSrcData) override;

	void __stdcall VSSetting(uint32_t slot) override;

	void __stdcall PSSetting(uint32_t slot) override;

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11Buffer* pBuffer_;
};