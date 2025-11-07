#include "stdafx.h"
#include "ImguiWidget.h"

ImguiWidget::ImguiWidget()
	:refCount_(1),
	isActive_(true)
{
	managerLink_.prev_ = nullptr;
	managerLink_.next_ = nullptr;
	managerLink_.item_ = this;
}

ImguiWidget::~ImguiWidget()
{
	CleanUp();
}

HRESULT __stdcall ImguiWidget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImguiWidget::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImguiWidget::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool ImguiWidget::IsActive() const
{
	return isActive_;
}

void ImguiWidget::OnActive()
{
	isActive_ = true;
}

void ImguiWidget::OffActive()
{
	isActive_ = false;
}


LINK_ITEM* ImguiWidget::MangerLink()
{
	return &managerLink_;
}

void ImguiWidget::CleanUp()
{

}