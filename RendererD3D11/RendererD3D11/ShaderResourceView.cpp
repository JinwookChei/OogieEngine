#include "stdafx.h"
//#include "ShaderResourceView.h"
//
//ShaderResourceView::ShaderResourceView()
//	:refCount_(1),
//	type_(EViewType::ShaderResourceView),
//	pShaderResourceView_(nullptr)
//{
//}
//
//ShaderResourceView::~ShaderResourceView()
//{
//	CleanUp();
//}
//
//bool ShaderResourceView::Init(ID3D11ShaderResourceView* pShaderResourceView)
//{
//	if (nullptr == pShaderResourceView)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	pShaderResourceView_ = pShaderResourceView;
//	return true;
//}
//
//HRESULT __stdcall ShaderResourceView::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall ShaderResourceView::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall ShaderResourceView::Release()
//{
//	--refCount_;
//	ULONG tmpRefCount = refCount_;
//	if (0 == refCount_) {
//		delete this;
//	}
//
//	return tmpRefCount;
//}
//
//void* __stdcall ShaderResourceView::GetView()
//{
//	return pShaderResourceView_;
//}
//
//void ShaderResourceView::CleanUp()
//{
//	if (nullptr != pShaderResourceView_)
//	{
//		pShaderResourceView_->Release();
//		pShaderResourceView_ = nullptr;
//	}
//}
