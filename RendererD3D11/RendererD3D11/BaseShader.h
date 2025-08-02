#pragma once

class BaseShader
	: public IShader
{
public:
	BaseShader();

	virtual ~BaseShader();

	//virtual ~BaseShader() = 0
	//{
	//	CleanUp();
	//}

	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

	ULONG __stdcall AddRef() override;

	ULONG __stdcall Release() override;

	void __stdcall Setting() override;

	virtual bool CreateShader(ID3DBlob* pBlob) = 0;

	virtual void SetShader() = 0;

	ID3DBlob* GetBlob() const;

	virtual void CleanUp() = 0;

protected:
	ID3DBlob* pBlob_;

private:
	UINT refCount_;

	
};