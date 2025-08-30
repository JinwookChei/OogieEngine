#include "stdafx.h"
#include "Application.h"
#include "RenderDevice.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Test.h"
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"

InputManager* GInputManager = nullptr;
TimeManager* GTimeManager = nullptr;
Camera* GCamera = nullptr;

Engine::Engine()
	: pStartUp_(nullptr),
	pWorld_(nullptr),
	pApplication_(nullptr),
	pRenderDevice_(nullptr)
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

	if (false == LoadRenderDevice())
	{
		return false;
	}


	GInputManager = new InputManager;
	if (nullptr == GInputManager)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == GInputManager->Initialize())
	{
		return false;
	}

	GTimeManager = new TimeManager;
	if (nullptr == GTimeManager)
	{
		DEBUG_BREAK();
		return false;
	}

	GCamera = new Camera;
	if (nullptr == GCamera)
	{
		DEBUG_BREAK();
		return false;
	}
	GCamera->BeginPlay();

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

		// Render
		GCamera->Tick(deltaTime);

		pRenderDevice_->RenderBegin();

		pWorld_->OnRender();

		pRenderDevice_->RenderEnd();
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

bool Engine::LoadRenderDevice()
{
	pRenderDevice_ = RenderDevice::Create();

	if (nullptr == pRenderDevice_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr == pApplication_)
	{
		DEBUG_BREAK();
		return false;
	}

	void* pMainHwnd = pApplication_->GetMainWindowHandle();

	LPCWSTR libFileName = L"RendererD3D11_x64_Debug.dll";

	return pRenderDevice_->Load(pMainHwnd, libFileName);
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
	if (nullptr == pWorld_)
	{
		DEBUG_BREAK();
		return false;
	}

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

	if (nullptr != GCamera)
	{
		delete GCamera;
		GCamera = nullptr;
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

	RenderDevice::Destroy();

	Application::Destroy();

}
