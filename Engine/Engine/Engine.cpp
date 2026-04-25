#include "stdafx.h"
#include "StartUp.h"
#include "RunTimeMode.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"


SkeletonManager* GSkeletonManager = nullptr;
MeshManager* GMeshManager = nullptr;
MaterialManager* GMaterialManager = nullptr;
TextureManager* GTextureManager = nullptr;
AnimationManager* GAnimationManager = nullptr;
CameraManager* GCameraManager = nullptr;

World* GWorld = nullptr;
ActorPicker* GActorPicker = nullptr;

// 임시 전역 객체.
IPSO* GBlitPSO = nullptr;

Engine::Engine()
	: pStartUp_(nullptr)
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

	float dpiSclae = Editor::GetEditor()->EnableDpiAwareness();
	if (false == Application::LoadApplication(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == Renderer::LoadRenderer())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == FBXManager::LoadFBXImporter())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == Editor::GetEditor()->Init(Application::GApplication, Renderer::GRenderer, dpiSclae))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InputManager::Init())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == TimeManager::Init())
	{
		return false;
	}

	if (false == CameraManager::Init())
	{
		return false;
	}

	GMeshManager = new MeshManager;
	GSkeletonManager = new SkeletonManager;
	GTextureManager = new TextureManager;
	GMaterialManager = new MaterialManager;
	GAnimationManager = new AnimationManager;
	RunTimeMode::Create();
	Debugger::Create();

	if (false == InitializeStartUp(pStartup))
	{
		DEBUG_BREAK();
		return false;
	}

	GWorld = new World;
	GActorPicker = new ActorPicker;
	return true;
}

void Engine::Run()
{
	// TEMP
	MeshManager::Instance()->TestLoad();
	TextureManager::Instance()->TestLoad();
	MaterialManager::Instance()->TestLoad();
	AnimationManager::Instance()->TestLoad();
	FBXManager::TestLoad();
	TimeManager::StartTimer();

	IMesh* pBlitMesh = nullptr;
	MeshManager::Instance()->GetMesh(&pBlitMesh, 0);
	IMaterial* pBlitMaterial = nullptr;
	MaterialManager::Instance()->GetMaterial(&pBlitMaterial, 3);
	PipelineStateDesc blitDesc;
	blitDesc.depthState = E_DEPTH_PRESET::DEPTH_DISABLE;
	blitDesc.rasterizerMode = E_RASTERIZER_PRESET::DISABLE;
	blitDesc.materialSlotCount = 1;
	blitDesc.meshSlotCount = 1;
	GBlitPSO = Renderer::GetFactory()->CreatePipelineStateObject(blitDesc);
	GBlitPSO->SetMeshToSlot(0, pBlitMesh);
	GBlitPSO->SetMaterialToSlot(0, pBlitMaterial);

	while (false == Application::Instance()->ApplicationQuit()) 
	{
		Application::Instance()->WinPumpMessage();

		double deltaTime = TimeManager::CalcDeltaTime();
		InputManager::Tick(deltaTime);
		GActorPicker->Tick(deltaTime);

		// GameLoop
		GWorld->CheckChangeLevel();
		GWorld->OnTick(deltaTime);
		GWorld->OnRender();
		// GameLoop End

		if (InputManager::IsDown(VK_F2) || Editor::GetEditor()->GetGameModeFlag())
		{
			RunTimeMode::ToggleRunTimeMode();
		}
		if (RunTimeMode::GetCurrentMode() == E_RUNTIME_MODE::EDITOR)
		{
			Editor::GetEditor()->OnBegin();
		}

		// Blit RenderTarget 
		Renderer::Instance()->RenderBegin();
		switch (RunTimeMode::GetCurrentMode())
		{
		case E_RUNTIME_MODE::GAME:
			GWorld->OnBlit(); 
			break;
		case E_RUNTIME_MODE::EDITOR:
			Editor::GetEditor()->OnRender();
			break;
		default:
			break;
		}
		Renderer::Instance()->RenderEnd();
		// Blit RenderTarget  End

		Application::Instance()->EndFrame();
	}
}

World* Engine::GetWorld() const
{
	return GWorld;
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


void Engine::CleanUp()
{
	Editor::GetEditor()->Release();

	if (nullptr != GBlitPSO)
	{
		GBlitPSO->Release();
		GBlitPSO = nullptr;
	}

	if (nullptr != GActorPicker)
	{
		delete GActorPicker;
		GActorPicker = nullptr;
	}

	if (nullptr != GWorld)
	{
		delete GWorld;
		GWorld = nullptr;
	}

	if (nullptr != pStartUp_)
	{
		pStartUp_->Release();
		pStartUp_ = nullptr;
	}

	if (nullptr != GSkeletonManager)
	{
		delete GSkeletonManager;
		GSkeletonManager = nullptr;
	}

	if (nullptr != GAnimationManager)
	{
		delete GAnimationManager;
		GAnimationManager = nullptr;
	}

	if (nullptr != GTextureManager)
	{
		delete GTextureManager;
		GTextureManager = nullptr;
	}

	if (nullptr != GMaterialManager)
	{
		delete GMaterialManager;
		GMaterialManager = nullptr;
	}
	if (nullptr != GMeshManager)
	{
		delete GMeshManager;
		GMeshManager = nullptr;
	}

	TimeManager::CleanUp();
	InputManager::CleanUp();
	CameraManager::CleanUp();

	RunTimeMode::ShutDown();
	Debugger::ShutDown();
	FBXManager::ShutDown();
	Renderer::ShutDown();
	Application::ShutDown();
}