#pragma once

class BaseShader
	: public IShader
{
public:
	BaseShader();

	virtual ~BaseShader();

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	bool CreateShader(ID3DBlob* blob);

	virtual void SetShader() = 0;

	void* GetBufferPointer();

	size_t GetBufferSize();

protected:
	virtual bool OnCreateShader(ID3DBlob* blob) = 0;

	virtual void CleanUp();

private:
	ULONG refCount_;

	ID3DBlob* blob_;
};