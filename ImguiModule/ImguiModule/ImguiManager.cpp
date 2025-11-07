#include "stdafx.h"
#include "ImguiManager.h"
#include "ImguiWidget.h"
#include "ImguiTextureWidget.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImguiManager* GImguiManager = nullptr;
namespace ImguiSystem
{
	IMGUI_API IImguiManager* GetImguiManager()
	{
		if (nullptr == GImguiManager)
		{
			GImguiManager = new ImguiManager;
		}
		return GImguiManager;
	}
}


ImguiManager::ImguiManager()
	:refCount_(1),
	pApplication_(nullptr),
	pRenderer_(nullptr),
	dpiScale_(0.0f),
	pWidgetHead_(nullptr),
	pWidgetTail_(nullptr)
{
}

ImguiManager::~ImguiManager()
{
	CleanUp();
}

HRESULT __stdcall ImguiManager::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImguiManager::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImguiManager::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

float __stdcall ImguiManager::EnableDpiAwareness()
{
	ImGui_ImplWin32_EnableDpiAwareness();
	float dpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	return dpiScale;
}

bool __stdcall ImguiManager::InitImgui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Control
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 24.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(dpiScale);
	if (false == ImGui_ImplWin32_Init(pApplication->GetMainWindowHandle()))
	{
		DEBUG_BREAK();
		return false;
	}
	if (false == ImGui_ImplDX11_Init((ID3D11Device*)pRenderer->GetDeviceHandle(), (ID3D11DeviceContext*)pRenderer->GetDeviceContextHandle()))
	{
		DEBUG_BREAK();
		return false;
	}

	pApplication_ = pApplication;
	pApplication_->AddRef();
	pRenderer_ = pRenderer;
	pRenderer_->AddRef();

	return true;
}

bool __stdcall ImguiManager::CreateWidget(const ImguiWidgetDesc& desc)
{
	switch (desc.widgetType)
	{
	case EWidgetType::TextureWidget:
	{
		ImTextureID* pSRV = static_cast<ImTextureID*>(desc.textureResourceView);
		if (nullptr == pSRV)
		{
			DEBUG_BREAK();
			return false;
		}

		ImguiWidget* pNewWidget = new ImguiTextureWidget(pSRV, desc.text);
		LinkToLinkedListFIFO(&pWidgetHead_, &pWidgetTail_, pNewWidget->MangerLink());
	}
	break;
	default:
		break;
	}

	return false;
}

void __stdcall ImguiManager::CleanUpImgui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (nullptr != GImguiManager)
	{
		GImguiManager->Release();
		GImguiManager = nullptr;
	}
}

bool g_ShowExitPopup = false;
bool g_bShow_demo_window = true;
bool g_bShow_another_window = false;
ImVec4 g_Imgui_Clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void __stdcall ImguiManager::OnRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ESC 키 감지
	if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	{
		g_ShowExitPopup = true;
		ImGui::OpenPopup("Exit Program");
	}
	// 종료 팝업
	if (g_ShowExitPopup)
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
				g_ShowExitPopup = false;
			}

			ImGui::EndPopup();
		}
	}


	LINK_ITEM* pWidgetIter = pWidgetHead_;
	while (pWidgetIter)
	{
		ImguiWidget* pCurWidget = static_cast<ImguiWidget*>(pWidgetIter->item_);
		pWidgetIter = pWidgetIter->next_;

		if (pCurWidget->IsActive())
		{
			pCurWidget->Render();
		}
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool __stdcall ImguiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
}



void ImguiManager::CleanUp()
{
	CleanUpWigets();

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

void ImguiManager::CleanUpWigets()
{
	while (pWidgetHead_)
	{
		ImguiWidget* pCurWidget = static_cast<ImguiWidget*>(pWidgetHead_->item_);
		UnLinkFromLinkedList(&pWidgetHead_, &pWidgetTail_, pCurWidget->MangerLink());

		pCurWidget->Release();
		pCurWidget = nullptr;
	}
}


// Regacy
//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
//if (g_bShow_demo_window)
//{
//	ImGui::ShowDemoWindow(&g_bShow_demo_window);
//}
//// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
//{
//	static float f = 0.0f;
//	static int counter = 0;
//	ImGuiIO& io = ImGui::GetIO();
//	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//	ImGui::Checkbox("Demo Window", &g_bShow_demo_window);      // Edit bools storing our window open/close state
//	ImGui::Checkbox("Another Window", &g_bShow_another_window);
//	ImGui::SliderFloat("float", &f, 0.0, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//	ImGui::ColorEdit3("clear color", (float*)&g_Imgui_Clear_color); // Edit 3 floats representing a color
//	if (ImGui::Button("Button"))
//	{ // Buttons return true when clicked (most widgets return true when edited/activated)
//		counter++;
//	}
//	ImGui::SameLine();
//	ImGui::Text("counter = %d", counter);
//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//	ImGui::End();
//}
//// 3. Show another simple window.
//if (g_bShow_another_window)
//{
//	ImGui::Begin("Another Window", &g_bShow_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
//	ImGui::Text("Hello from another window!");
//	if (ImGui::Button("Close Me"))
//	{
//		g_bShow_another_window = false;
//	}
//	ImGui::End();
//}