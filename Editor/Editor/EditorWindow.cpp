#include "stdafx.h"
#include "EditorWindow.h"

EditorWindow::EditorWindow()
	: refCount_(1)
	, flags_(ImGuiWindowFlags_None)
	, state_(eState::Active)
	, size_(0.0f, 0.0f)
{
}

EditorWindow::~EditorWindow()
{
	CleanUp();
}

HRESULT __stdcall EditorWindow::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall EditorWindow::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall EditorWindow::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

void EditorWindow::OnRender()
{
	Begin();

	Update();
	Render();

	End();
}

void EditorWindow::Begin()
{
	bool Active = (bool)GetState();
	ImGui::Begin(GetName().c_str(), &Active, GetFlag());
}

void EditorWindow::End()
{
	ImGui::End();
}



void EditorWindow::CleanUp()
{

}