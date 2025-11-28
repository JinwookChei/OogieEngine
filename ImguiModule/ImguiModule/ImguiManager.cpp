#include "stdafx.h"
#include "GBufferViewer.h"
#include "ImGuiTextureWidget.h"
#include "ExitPopup.h"
#include "ImGuiManager.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ImGuiManager* GImGuiManager = nullptr;
namespace ImGuiSystem
{
	IMGUI_API IImGuiManager* GetImGuiManager()
	{
		if (nullptr == GImGuiManager)
		{
			GImGuiManager = new ImGuiManager;
		}
		return GImGuiManager;
	}
}


ImGuiManager::ImGuiManager()
	:refCount_(1),
	pApplication_(nullptr),
	pRenderer_(nullptr),
	dpiScale_(0.0f),
	pExitPopup_(nullptr),
	pGBufferViewer_(nullptr)
{
}

ImGuiManager::~ImGuiManager()
{
	CleanUp();
}

HRESULT __stdcall ImGuiManager::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ImGuiManager::AddRef(void)
{
	return ++refCount_;
}

ULONG __stdcall ImGuiManager::Release(void)
{
	--refCount_;
	ULONG tempRefCount = refCount_;
	if (0 == refCount_)
	{
		delete this;
	}
	return tempRefCount;
}

float __stdcall ImGuiManager::EnableDpiAwareness()
{
	ImGui_ImplWin32_EnableDpiAwareness();
	float dpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	return dpiScale;
}

bool __stdcall ImGuiManager::InitImGui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale)
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


	pExitPopup_ = ExitPopup::Create(pApplication_);
	return true;
}

void __stdcall ImGuiManager::CleanUpImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (nullptr != GImGuiManager)
	{
		GImGuiManager->Release();
		GImGuiManager = nullptr;
	}
}

void __stdcall ImGuiManager::OnRender()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (nullptr != pExitPopup_)
	{
		pExitPopup_->Render();
	}

	if (nullptr != pGBufferViewer_)
	{
		pGBufferViewer_->Render();
	}

	
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

bool __stdcall ImGuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return true;
	}
}

bool __stdcall ImGuiManager::BindCamera(IImGuiBindCamera* pCamera)
{
	if (nullptr == pCamera)
	{
		Assert("Camera is NULL");
		return false;
	}

	if (nullptr != pGBufferViewer_)
	{
		pGBufferViewer_->Release();
		pGBufferViewer_ = nullptr;
	}

	pGBufferViewer_ = GBufferViewer::Create(pCamera);
	pGBufferViewer_->OnActive();

	return true;
}

bool __stdcall ImGuiManager::BindPickedActor(IImGuiBindPickedActor* pPickedActor)
{
	return false;
}



void ImGuiManager::CleanUp()
{
	if (nullptr != pExitPopup_)
	{
		pExitPopup_->Release();
		pExitPopup_ = nullptr;
	}
	if (nullptr != pGBufferViewer_)
	{
		pGBufferViewer_->Release();
		pGBufferViewer_ = nullptr;
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