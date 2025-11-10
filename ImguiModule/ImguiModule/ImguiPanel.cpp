#include "stdafx.h"
#include "ImGuiPanel.h"

ImGuiPanel::ImGuiPanel()
	: refCount_(1),
	isActive_(true)
{
}

ImGuiPanel::~ImGuiPanel()
{
	CleanUp();
}

HRESULT __stdcall ImGuiPanel::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImGuiPanel::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImGuiPanel::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool ImGuiPanel::IsActive() const
{
	return isActive_;
}

void ImGuiPanel::OnActive()
{
	isActive_ = true;
}

void ImGuiPanel::OffActive()
{
	isActive_ = false;
}

void ImGuiPanel::CleanUp()
{

}