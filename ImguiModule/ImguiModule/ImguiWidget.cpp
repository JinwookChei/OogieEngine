#include "stdafx.h"
#include "ImGuiWidget.h"

ImGuiWidget::ImGuiWidget()
	:refCount_(1),
	isActive_(true)
{
	managerLink_.prev_ = nullptr;
	managerLink_.next_ = nullptr;
	managerLink_.item_ = this;
}

ImGuiWidget::~ImGuiWidget()
{
	CleanUp();
}

HRESULT __stdcall ImGuiWidget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImGuiWidget::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImGuiWidget::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool ImGuiWidget::IsActive() const
{
	return isActive_;
}

void ImGuiWidget::OnActive()
{
	isActive_ = true;
}

void ImGuiWidget::OffActive()
{
	isActive_ = false;
}


LINK_ITEM* ImGuiWidget::MangerLink()
{
	return &managerLink_;
}

void ImGuiWidget::CleanUp()
{

}