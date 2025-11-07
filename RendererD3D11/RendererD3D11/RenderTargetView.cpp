#include "stdafx.h"
//#include "RenderTargetView.h"
//
//RenderTargetView::RenderTargetView()
//	:refCount_(1),
//	type_(EViewType::RenderTargetView),
//	pRenderTargetView_(nullptr)
//{
//}
//
//RenderTargetView::~RenderTargetView()
//{
//	CleanUp();
//}
//
//bool RenderTargetView::Init(ID3D11RenderTargetView* pRenderTagetView)
//{
//	if (nullptr == pRenderTagetView)
//	{
//		DEBUG_BREAK();
//		return false;
//	}
//
//	pRenderTargetView_ = pRenderTagetView;
//	return true;
//}
//
//HRESULT __stdcall RenderTargetView::QueryInterface(REFIID riid, void** ppvObject)
//{
//	return E_NOTIMPL;
//}
//
//ULONG __stdcall RenderTargetView::AddRef()
//{
//	return ++refCount_;
//}
//
//ULONG __stdcall RenderTargetView::Release()
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
//void* __stdcall RenderTargetView::GetView()
//{
//	return pRenderTargetView_;
//}
//
//void RenderTargetView::CleanUp()
//{
//	if (nullptr != pRenderTargetView_)
//	{
//		pRenderTargetView_->Release();
//		pRenderTargetView_ = nullptr;
//	}
//}
