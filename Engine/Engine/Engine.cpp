#include "stdafx.h"
#include "Application.h"
#include "RenderDevice.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"
#include "InputLayout.h"

InputManager* GInputManager = nullptr;
TimeManager* GTimeManager = nullptr;
ConstantManager* GConstantManager = nullptr;

Camera* GCurrentCamera = nullptr;
//Camera* GCamera1 = nullptr;
//Camera* GCamera2 = nullptr;

SpotLight* GSpotLight = nullptr;


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
	if (false == GInputManager->Initialize())
	{
		return false;
	}

	GTimeManager = new TimeManager;

	//GCamera1 = new Camera;
	//GCamera1->BeginPlay();
	//GCamera1->SetClearColor({0.0f, 0.0f, 0.6f ,1.0f});


	//GCamera2 = new Camera;
	//GCamera2->BeginPlay();
	//GCamera2->SetClearColor({0.0f, 0.6f, 0.0f,1.0f });
	

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

		//GCamera1->Tick(deltaTime);

		pWorld_->OnTick(deltaTime);

		//GCurrentCamera = GCamera1;
		//GConstantManager->Update();
		//GCamera1->RenderTest();
		//pWorld_->OnRender();
		//
		//GCurrentCamera = GCamera2;
		//GConstantManager->Update();
		//GCamera2->RenderTest();
		pWorld_->OnRender();
		
		// Render
		pRenderDevice_->RenderBegin();
		
		pWorld_->OnBlit();
		//GCamera1->BlitToBackBuffer({ -0.5f, 0.0f }, {0.5f, 1.0f});

		//GCamera2->BlitToBackBuffer({ 0.5f, 0.0f }, { 0.5f, 1.0f });
		
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

	//if (nullptr != GCamera1)
	//{
	//	delete GCamera1;
	//	GCamera1 = nullptr;
	//}
	//if (nullptr != GCamera2)
	//{
	//	delete GCamera2;
	//	GCamera2 = nullptr;
	//}

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
