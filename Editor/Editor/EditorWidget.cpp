#include "stdafx.h"
#include "EditorWIdget.h"


EditorWidget::EditorWidget()
	:refCount_(1)
{
}

EditorWidget::~EditorWidget()
{
	CleanUp();
}

HRESULT __stdcall EditorWidget::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall EditorWidget::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall EditorWidget::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}


void EditorWidget::CleanUp()
{

}