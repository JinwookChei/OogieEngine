#pragma once

//class InputLayout
//	: public IInputLayout
//{
//public:
//	InputLayout();
//
//	virtual ~InputLayout();
//
//	HRESULT __stdcall QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;
//
//	ULONG __stdcall AddRef() override;
//
//	ULONG __stdcall Release() override;
//
//	//bool AddInputLayout(const char* semanticName, unsigned int semanticIndex, unsigned int format, unsigned int inputSlot, bool isInstanceData) override;
//
//	UINT FormatSize(DXGI_FORMAT format);
//
//private:
//	UINT refCount_;
//
//	UINT offset_;
//
//	std::vector<D3D11_INPUT_ELEMENT_DESC> desc_;
//};
