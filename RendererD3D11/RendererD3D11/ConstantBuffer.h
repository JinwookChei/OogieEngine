#pragma once

class ConstantBuffer final
	: public IUnknown
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

	void Update(void* pSrcData);

	void BindConstantBufferCS(UINT slot);
	void BindConstantBufferVS(UINT slot);
	void BindConstantBufferGS(UINT slot);
	void BindConstantBufferPS(UINT slot);

private:
	void CleanUp();

	ULONG refCount_;

	ID3D11Buffer* pBuffer_;
};