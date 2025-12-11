#pragma once

class ConstantBuffer final
	: public IConstantBuffer
{
private:
	friend Renderer;

	ConstantBuffer();

	virtual ~ConstantBuffer();

	bool Init(ID3D11Buffer* pBuffer);

	bool Init(uint32_t bufferSize);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	static ConstantBuffer* Create(uint32_t bufferSize);

	void __stdcall Update(void* pSrcData) override;

	void BindConstantBufferCS(UINT slot);
	void BindConstantBufferVS(UINT slot);
	void BindConstantBufferGS(UINT slot);
	void BindConstantBufferPS(UINT slot);

	void __stdcall VSSetting(uint32_t slot) override;

	void __stdcall PSSetting(uint32_t slot) override;

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11Buffer* pBuffer_;
};