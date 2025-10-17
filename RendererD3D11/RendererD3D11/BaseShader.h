#pragma once

class BaseShader
	: public IShader
{
protected:
	friend class Renderer;

	BaseShader();

	virtual ~BaseShader();

	bool Init(ID3DBlob* pBlob);

public:
	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void Setting();

	void* GetBufferPointer();

	size_t GetBufferSize();

protected:
	virtual void SetShader() = 0;

	virtual bool OnCreateShader(ID3DBlob* pBlob) = 0;

	virtual void CleanUp();

private:
	ULONG refCount_;

	ID3DBlob* pBlob_;
};