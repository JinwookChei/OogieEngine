//#pragma once
//
//class ShaderResourceView : public IView
//{
//private:
//	friend Renderer;
//
//	ShaderResourceView();
//
//	virtual ~ShaderResourceView();
//
//	bool Init(ID3D11ShaderResourceView* pShaderResourceView);
//
//public:
//	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
//
//	ULONG __stdcall AddRef() override;
//
//	ULONG __stdcall Release() override;
//
//	void* __stdcall GetView() override;
//
//private:
//	void CleanUp();
//
//private:
//	ULONG refCount_;
//
//	EViewType type_;
//
//	ID3D11ShaderResourceView* pShaderResourceView_;
//};
