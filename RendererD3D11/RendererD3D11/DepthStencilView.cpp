#include "stdafx.h"
//#include "DepthStencilView.h"
//
//DepthStencilView::DepthStencilView()
//	:refCount_(1),
//	type_(EViewType::DepthStencilView),
//	pDepthStencilView_(nullptr)
//{
//}
//
//DepthStencilView::~DepthStencilView()
//{
//	CleanUp();
//}
//
//bool DepthStencilView::Init(ID3D11DepthStencilView* pDepthStencilView)
//{
//	if (nullptr == pDepthStencilView)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	pDepthStencilView_ = pDepthStencilView;
//	return true;
//}
//
//HRESULT __stdcall DepthStencilView::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall DepthStencilView::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall DepthStencilView::Release()
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
//void* __stdcall DepthStencilView::GetView()
//{
//	return pDepthStencilView_;
//}
//
//void DepthStencilView::CleanUp()
//{
//	if (nullptr != pDepthStencilView_)
//	{
//		pDepthStencilView_->Release();
//		pDepthStencilView_ = nullptr;
//	}
//}
