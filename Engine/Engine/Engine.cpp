#include "stdafx.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);
typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);


InputManager* GInputManager = nullptr;
IApplication* GApplication = nullptr;
IRenderer* GRenderer = nullptr;
TimeManager* GTimeManager = nullptr;
ConstantManager* GConstantManager = nullptr;
Camera* GCurrentCamera = nullptr;
IBlendState* GBlendState = nullptr;

Engine::Engine()
	: pStartUp_(nullptr),
	pWorld_(nullptr),
	applicationModule_(),
	pApplication_(nullptr),
	rendererModule_(),
	pRenderer_(nullptr),
	pImguiManager_(nullptr)
{
}

Engine::~Engine()
{
	CleanUp();
}

Engine* Engine::CreateEngine()
{
	return new Engine;
}

void Engine::DestroyEngine(Engine* pEngine)
{
	if (nullptr == pEngine)
	{
		DEBUG_BREAK();
		return;
	}

	delete pEngine;
}

bool Engine::Initialize
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath,
	IStartup* pStartup
)
{
	if (nullptr == pStartup)
	{
		DEBUG_BREAK();
		return false;
	}

	float dpiSclae = ImguiSystem::GetImguiManager()->EnableDpiAwareness();

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath))
	{
		return false;
	}

	if (false == LoadRenderer())
	{
		return false;
	}

	if (false == ImguiSystem::GetImguiManager()->InitImgui(pApplication_, pRenderer_, dpiSclae))
	{
		return false;
	}

	GInputManager = new InputManager;
	if (false == GInputManager->Initialize())
	{
		return false;
	}

	GTimeManager = new TimeManager;

	GConstantManager = new ConstantManager;

	if (false == InitializeStartUp(pStartup))
	{
		return false;
	}

	if (false == InitializeWorld())
	{
		return false;
	}

	GBlendState = GRenderer->CreateBlendState(2, 2, 2, 2, nullptr);
	if (nullptr == GBlendState)
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	while (false == pApplication_->ApplicationQuit()) {

		pApplication_->WinPumpMessage();

		// Calc DeltaTime
		double deltaTime = GTimeManager->CalcDeltaTime();

		// Input Update
		GInputManager->Tick(deltaTime);

		// GameLoop
		pWorld_->CheckChangeLevel();

		pWorld_->OnTick(deltaTime);

		pWorld_->OnRender();
		// GameLoop End

		// Blit RenderTarget 
		pRenderer_->RenderBegin();

		pWorld_->OnBlit();

		ImguiSystem::GetImguiManager()->RenderImgui();

		pRenderer_->RenderEnd();
		// Blit RenderTarget  End
	}
}

World* Engine::GetWorld() const
{
	return pWorld_;
}

bool Engine::LoadApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath
)
{
	if (nullptr != pApplication_)
	{
		DEBUG_BREAK();
		pApplication_->Release();
		pApplication_ = nullptr;
	}

	LPCWSTR libFileName = L"Application_x64_Debug.dll";
	applicationModule_ = LoadLibrary(libFileName);
	if (!applicationModule_)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG5 CreateWindowsApplication = (DLL_FUNCTION_ARG5)GetProcAddress(applicationModule_, "CreateWindowsApplication");
	if (!CreateWindowsApplication)
	{
		DEBUG_BREAK();
		return false;
	}


	CreateWindowsApplication
	(
		(void**)&pApplication_, 
		hInstance,
		pCmdLine, 
		nCmdShow, 
		pIConPath
	);

	if (nullptr == pApplication_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pApplication_->InitializeMainWindow(pMainWindowClassName, pMainWindowText, {DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT}))
	{
		DEBUG_BREAK();
		return false;
	}

	GApplication = pApplication_;

	return true;
}

bool Engine::LoadRenderer()
{
	if (nullptr == pApplication_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr != pRenderer_)
	{
		DEBUG_BREAK();
		pRenderer_->Release();
		pRenderer_ = nullptr;
	}

	LPCWSTR libFileName = L"RendererD3D11_x64_Debug.dll";
	rendererModule_ = LoadLibrary(libFileName);
	if (!rendererModule_)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateRenderer = (DLL_FUNCTION_ARG1)GetProcAddress(rendererModule_, "CreateRenderer");
	if (!CreateRenderer)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateRenderer((void**)&pRenderer_);
	if (nullptr == pRenderer_)
	{
		DEBUG_BREAK();
		return false;
	}

	void* pMainHwnd = pApplication_->GetMainWindowHandle();
	if (false == pRenderer_->Initialize(pMainHwnd, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT))
	{
		DEBUG_BREAK();
		return false;
	}

	GRenderer = pRenderer_;
	return true;
}

bool Engine::InitializeStartUp(IStartup* startUp)
{
	if (nullptr == startUp)
	{
		DEBUG_BREAK();
		return false;
	}

	pStartUp_ = startUp;
	pStartUp_->AddRef();

	if (false == pStartUp_->Start())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Engine::InitializeWorld()
{
	pWorld_ = new World;

	return true;
}

void Engine::CleanUp()
{
	ImguiSystem::GetImguiManager()->CleanUpImgui();

	if (nullptr != pWorld_)
	{
		delete pWorld_;
		pWorld_ = nullptr;
	}

	if (nullptr != pStartUp_)
	{
		pStartUp_->Release();
		pStartUp_ = nullptr;
	}

	if (nullptr != GConstantManager)
	{
		delete GConstantManager;
		GConstantManager = nullptr;
	}

	if (nullptr != GTimeManager)
	{
		delete GTimeManager;
		GTimeManager = nullptr;
	}

	if (nullptr != GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
	}

	if (nullptr != pApplication_)
	{
		pApplication_->Release();
		pApplication_ = nullptr;
	}

	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}

	if (nullptr != GBlendState)
	{
		GBlendState->Release();
		GBlendState = nullptr;
	}

	if (nullptr != pRenderer_)
	{
		pRenderer_->Release();
		pRenderer_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
}

float Engine::EnableDpiAwareness()
{
	//ImGui_ImplWin32_EnableDpiAwareness();
	//float dpiScale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));
	//return dpiScale;
	return 1.5f;
}

bool Engine::InitImgui(IApplication* pApplication, IRenderer* pRenderer, float dpiScale)
{
	//// Setup Dear ImGui context
	//IMGUI_CHECKVERSION();
	//ImGui::CreateContext();
	//ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Control
	//io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 24.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
	//ImGui::StyleColorsDark();
	//ImGuiStyle& style = ImGui::GetStyle();
	//style.ScaleAllSizes(dpiScale);
	//if (false == ImGui_ImplWin32_Init(pApplication->GetMainWindowHandle()))
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}
	//if (false == ImGui_ImplDX11_Init((ID3D11Device*)pRenderer->GetDeviceHandle(), (ID3D11DeviceContext*)pRenderer->GetDeviceContextHandle()))
	//{
	//	DEBUG_BREAK();
	//	return false;
	//}

	return true;
}

void Engine::CleanUpImgui()
{
	//ImGui_ImplDX11_Shutdown();
	//ImGui_ImplWin32_Shutdown();
	//ImGui::DestroyContext();
}

//bool g_ShowExitPopup = false;
//bool g_bShow_demo_window = true;
//bool g_bShow_another_window = false;
//ImVec4 g_Imgui_Clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void Engine::RenderImgui()
{
	//// Start the Dear ImGui frame
	//ImGui_ImplDX11_NewFrame();
	//ImGui_ImplWin32_NewFrame();
	//ImGui::NewFrame();

	//// ESC 키 감지
	//if (ImGui::IsKeyPressed(ImGuiKey_Escape))
	//{
	//	g_ShowExitPopup = true;
	//	ImGui::OpenPopup("Exit Program");
	//}

	//// 종료 팝업
	//if (g_ShowExitPopup)
	//{
	//	ImGui::SetNextWindowSize(ImVec2(300, 150));
	//	if (ImGui::BeginPopupModal("Exit Program", NULL, ImGuiWindowFlags_NoResize))
	//	{
	//		ImGui::Text(" Do you want to exit the program ? ");
	//		ImGui::Spacing();
	//		ImGui::Spacing();

	//		if (ImGui::Button("Quit", ImVec2(120, 0)))
	//		{
	//			pApplication_->Quit();
	//		}
	//		ImGui::SameLine();
	//		if (ImGui::Button("Cancel", ImVec2(120, 0)))
	//		{
	//			ImGui::CloseCurrentPopup();
	//			g_ShowExitPopup = false;
	//		}

	//		ImGui::EndPopup();
	//	}
	//}

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

	//// Rendering
	//ImGui::Render();
	//ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}