#pragma once

class BaseShader
	: public IShader
{
public:
	BaseShader();

	virtual ~BaseShader() = 0
	{
		//CleanUp();
	}

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	virtual bool CreateShader(void* shaderBytecode, size_t bytecodeLength) = 0;

	virtual void SetShader() = 0;

	virtual void CleanUp() = 0;

private:
	UINT refCount_;
};