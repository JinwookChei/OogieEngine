#include "stdafx.h"
#include "ImguiPanel.h"

ImguiPanel::ImguiPanel()
	: refCount_(1),
	isActive_(true)
{
}

ImguiPanel::~ImguiPanel()
{
	CleanUp();
}

HRESULT __stdcall ImguiPanel::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImguiPanel::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImguiPanel::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool ImguiPanel::IsActive() const
{
	return isActive_;
}

void ImguiPanel::OnActive()
{
	isActive_ = true;
}

void ImguiPanel::OffActive()
{
	isActive_ = false;
}

void ImguiPanel::CleanUp()
{

}