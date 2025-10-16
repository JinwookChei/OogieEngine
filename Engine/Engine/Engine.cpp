#include "stdafx.h"
#include "Application.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);


InputManager* GInputManager = nullptr;
IRenderer* GRenderer = nullptr;
TimeManager* GTimeManager = nullptr;
ConstantManager* GConstantManager = nullptr;
Camera* GCurrentCamera = nullptr;
SpotLight* GSpotLight = nullptr;



Engine::Engine()
	: pStartUp_(nullptr),
	pWorld_(nullptr),
	pApplication_(nullptr),
	rendererModule_(),
	pRenderer_(nullptr)
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


	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath))
	{
		return false;
	}

	if (false == LoadRenderer())
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

	GSpotLight = new SpotLight;
	GSpotLight->BeginPlay();

	if (false == InitializeStartUp(pStartup))
	{
		return false;
	}

	if (false == InitializeWorld())
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

		// Render
		pRenderer_->RenderBegin();

		pWorld_->OnBlit();

		pRenderer_->RenderEnd();
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
	pApplication_ = Application::Create();

	if (nullptr == pApplication_)
	{
		DEBUG_BREAK();
		return false;
	}

	LPCWSTR libFileName = L"Application_x64_Debug.dll";
	return pApplication_->Load(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath, libFileName);
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
	if (false == pRenderer_->Initialize(pMainHwnd, 2560, 1440))
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

	if (nullptr != GSpotLight)
	{
		delete GSpotLight;
		GSpotLight = nullptr;
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

	Application::Destroy();
}
