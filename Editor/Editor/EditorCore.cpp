#include "stdafx.h"
#include "EditorContext.h"
#include "EditorWindow.h"
#include "EditorCore.h"
#include "InspectorWindow.h"
#include "SceneWindow.h"

bool bShowExitPopup = false;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EditorCore* GEditor = nullptr;
namespace Editor
{
	EDITOR_API IEditor* GetEditor()
	{
		if (nullptr == GEditor)
		{
			GEditor = new EditorCore;
		}
		return GEditor;
	}
}


EditorCore::EditorCore()
	: refCount_(1)
	, pApplication_(nullptr)
	, pRenderer_(nullptr)
	, dpiScale_(0.0f)
	, pEditorContext_(nullptr)
	, editorWindows_()
	, flags_(ImGuiWindowFlags_::ImGuiWindowFlags_None)
	, dockspaceFlags_(ImGuiDockNodeFlags_::ImGuiDockNodeFlags_None)
	, state_(eState::Active)
	, fullScreen_(true)
	, viewportSize_()
	, viewportFocused_(false)
	, viewportHovered_(false)
	, guizmoType_(-1)
	, pBoundCamera_(nullptr)
	, pPickedActor_(nullptr)
{
}

EditorCore::~EditorCore()
{
	CleanUp();
}

HRESULT __stdcall EditorCore::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall EditorCore::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall EditorCore::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

bool __stdcall EditorCore::Init(IApplication* pApplication, IRenderer* pRenderer, float dpiScale)
{
	pApplication_ = pApplication;
	pApplication_->AddRef();
	pRenderer_ = pRenderer;
	pRenderer_->AddRef();

	if (false == InitContext(pApplication_, pRenderer_, dpiScale))
	{
		return false;
	}

	if (false == InitWindows())
	{
		return false;
	}

	return true;
}

bool __stdcall EditorCore::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
}

float __stdcall EditorCore::EnableDpiAwareness()
{
	ImGui_ImplWin32_EnableDpiAwareness();
	float dpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	return dpiScale;
}


void __stdcall EditorCore::OnRender()
{
	pEditorContext_->Begin();

	Update();

	Render();

	pEditorContext_->End();
}

bool __stdcall EditorCore::IsWindowHovered(const char* name)
{
	ImGuiContext* ctx = ImGui::GetCurrentContext();
	if (!ctx || !name)
	{
		return false;
	}

	ImGuiWindow* hovered = ctx->HoveredWindow;
	if (!hovered)
	{
		return false;
	}

	return strcmp(hovered->Name, name) == 0;
}

Float2 __stdcall EditorCore::GetMousePos()
{
	ImGuiWindow* hovered = ImGui::GetCurrentContext()->HoveredWindow;
	if (hovered)
	{
		ImVec2 mouse = ImGui::GetMousePos();
		ImVec2 local = ImVec2
		(
			mouse.x - hovered->DC.CursorStartPos.x + hovered->Scroll.x,
			mouse.y - hovered->DC.CursorStartPos.y + hovered->Scroll.y
		);
		return { local.x , local.y };
	}
	return { -1, -1 };
}

Float2 __stdcall EditorCore::GetViewPortSize()
{
	ImGuiWindow* hovered = ImGui::GetCurrentContext()->HoveredWindow;
	if (hovered)
	{
		ImVec2 size = hovered->ContentRegionRect.GetSize();
		return { size.x, size.y };
	}
	return { -1, -1 };
}

bool EditorCore::InitContext(IApplication* pApplication, IRenderer* pRenderer, float dpiScale)
{
	pEditorContext_ = new EditorContext;
	return pEditorContext_->Init(pApplication, pRenderer, dpiScale);
}

bool EditorCore::InitWindows()
{
	//InspectorWindow
	InspectorWindow* pInspector = new InspectorWindow();
	editorWindows_.insert(std::make_pair(L"InspectorWindow", pInspector));
	//EventCallback = &EditorApplication::OnEvent;

	SceneWindow* pScene = new SceneWindow();
	editorWindows_.insert(std::make_pair(L"SceneWindow", pScene));

	return true;
}

void EditorCore::Update()
{
	// ESC 키 감지
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		bShowExitPopup = true;
		ImGui::OpenPopup("Exit Program");
	}
	// 종료 팝업
	if (bShowExitPopup)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 150));
		if (ImGui::BeginPopupModal("Exit Program", NULL, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text(" Do you want to exit the program ? ");
			ImGui::Spacing();
			ImGui::Spacing();

			if (ImGui::Button("Quit", ImVec2(120, 0)))
			{
				pApplication_->Quit();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				bShowExitPopup = false;
			}

			ImGui::EndPopup();
		}
	}
}


void EditorCore::Render()
{
	// 이 윈도우 자체가 다른 DockSpace 안에 “도킹되는 것”을 금지
	flags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (fullScreen_)
	{
		//ImGui Viewport를 얻어옴.
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		//ImGui 윈도우를 OS 창의 좌상단 좌표에 정확히 맞춤
		ImGui::SetNextWindowPos(viewport->Pos);
		//클라이언트 영역 전체를 덮음
		//메뉴바 + DockSpace + SceneView 전부 포함
		ImGui::SetNextWindowSize(viewport->Size);
		// 이 ImGui 윈도우는 반드시 메인 뷰포트에 속해야 함
		ImGui::SetNextWindowViewport(viewport->ID);
		// 둥근 모서리 제거.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		// 테두리 제거.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		// Imgui의 윈도우 장식 제거 + 고정
		flags_ |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		flags_ |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	if (dockspaceFlags_ & ImGuiDockNodeFlags_PassthruCentralNode)
	{
		flags_ |= ImGuiWindowFlags_NoBackground;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	bool Active = static_cast<bool>(state_);
	ImGui::Begin("EditorApplication", &Active, flags_);
	ImGui::PopStyleVar();

	if (fullScreen_)
	{
		ImGui::PopStyleVar(2);
	}

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	//ImGuiStyle& style = ImGui::GetStyle();
	//float minWinSizeX = style.WindowMinSize.x;
	//style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspaceFlags_);
	}
	//style.WindowMinSize.x = minWinSizeX;



	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
	//			//OpenProject();

	//		ImGui::Separator();

	//		if (ImGui::MenuItem("New Scene", "Ctrl+N"))
	//			//NewScene();

	//		if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
	//			//SaveScene();

	//		if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
	//			//SaveSceneAs();

	//		ImGui::Separator();

	//		if (ImGui::MenuItem("Exit"))
	//			//application.Close();

	//		ImGui::EndMenu();
	//	}

	//	if (ImGui::BeginMenu("Script"))
	//	{
	//		if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
	//		{
	//			//ScriptEngine::ReloadAssembly(); 추후 C#스크립트 추가기능이 생기면 추가할 예정
	//		}

	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}

		//auto iter = EditorWindows.find(L"SceneWindow");
		//dynamic_cast<SceneWindow*>(iter->second)->SetGuizmoType(GuizmoType);

		//// check if the mouse,keyboard is on the Sceneview
		//ViewportFocused = ImGui::IsWindowFocused();
		//ViewportHovered = ImGui::IsWindowHovered();
		//ImguiEditor->BlockEvent(!ViewportHovered);

		//// viewport
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		//ImGui::Begin("Game");

		//// rendering framebuffer image to the gameview
		//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//ViewportSize = Vector2{ viewportPanelSize.x, viewportPanelSize.y };
		//ya::graphics::Texture* texture = FrameBuffer->GetAttachmentTexture(0);
		//ImGui::Image((ImTextureID)texture->GetSRV().Get(), ImVec2{ ViewportSize.x, ViewportSize.y }
		//, ImVec2{ 0, 0 }, ImVec2{ 1, 1 });

		//// Open Scene by drag and drop
		//if (ImGui::BeginDragDropTarget())
		//{
		//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("PROJECT_ITEM"))
		//	{
		//		const auto path = static_cast<const wchar_t*>(payload->Data);
		//		OpenScene(path);
		//	}
		//	ImGui::EndDragDropTarget();
		//}

	for (auto& iter : editorWindows_)
	{
		iter.second->OnRender();
	}

	ImGui::End(); // EditorApplication end
}


void EditorCore::CleanUp()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	for (auto iter : editorWindows_)
	{
		if (nullptr != iter.second)
		{
			iter.second->Release();
			iter.second = nullptr;
		}
	}

	if (nullptr != pEditorContext_)
	{
		pEditorContext_->Release();
		pEditorContext_ = nullptr;
	}

	if (nullptr != pApplication_)
	{
		pApplication_->Release();
		pApplication_ = nullptr;
	}

	if (nullptr != pRenderer_)
	{
		pRenderer_->Release();
		pRenderer_ = nullptr;
	}
}

bool __stdcall EditorCore::BindCamera(IEditorBindCamera* pCamera)
{
	if (nullptr == pCamera)
	{
		Assert("Camera is NULL");
		return false;
	}

	pBoundCamera_ = pCamera;
	return true;
}

bool __stdcall EditorCore::BindPickedActor(IEditorBindPickedActor* pPickedActor)
{
	pPickedActor_ = pPickedActor;
	return true;
}
