#include "stdafx.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);
typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);


IApplication* GApplication = nullptr;
IRenderer* GRenderer = nullptr;

MeshManager* GMeshManager = nullptr;
MaterialManager* GMaterialManager = nullptr;
TextureManager* GTextureManager = nullptr;
RasterizerManager* GRasterizerManager = nullptr;

World* GWorld = nullptr;
Camera* GMainCamera = nullptr;
Camera* GCurrentCamera = nullptr;
ActorPicker* GActorPicker = nullptr;

IParticle* GParticle_1 = nullptr;
IParticle* GParticle_2 = nullptr;


Engine::Engine()
	: pStartUp_(nullptr)
	, applicationModule_()
	, pApplication_(nullptr)
	, rendererModule_()
	, pRenderer_(nullptr)
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

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, pMainWindowClassName, pMainWindowText, pIConPath))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadRenderer())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == FBXManager::Init())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == Editor::GetEditor()->Init(pApplication_, pRenderer_, dpiSclae))
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

	GMeshManager = new MeshManager;

	GTextureManager = new TextureManager;

	GMaterialManager = new MaterialManager;

	GRasterizerManager = new RasterizerManager;

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
	MaterialManager::Instance()->TestLoad();
	TextureManager::Instance()->TestLoad();

	ParticleDesc particleDesc;
	particleDesc.maxNum_ = 10;
	particleDesc.patternType_ = E_PARTICLE_PATTERN_TYPE::EXPLOSION;
	TextureManager::Instance()->GetTexture(&particleDesc.pTexture_, 3);
	GParticle_1 = GRenderer->CreateParticle(particleDesc);

	ParticleDesc particleDesc2;
	particleDesc2.maxNum_ = 10;
	particleDesc2.patternType_ = E_PARTICLE_PATTERN_TYPE::FUME;
	TextureManager::Instance()->GetTexture(&particleDesc2.pTexture_, 3);
	GParticle_2 = GRenderer->CreateParticle(particleDesc2);
	

	while (false == pApplication_->ApplicationQuit()) {

		pApplication_->WinPumpMessage();

		// Calc DeltaTime
		double deltaTime = TimeManager::CalcDeltaTime();

		// Input Update
		InputManager::Tick(deltaTime);
		GActorPicker->Tick(deltaTime);

		GRenderer->UpdateParticles(GParticle_1, deltaTime);
		GRenderer->UpdateParticles(GParticle_2, deltaTime);
		
		// GameLoop
		GWorld->CheckChangeLevel();
		GWorld->OnTick(deltaTime);
		GWorld->OnRender();
		// GameLoop End

		// Blit RenderTarget 
		GRenderer->RenderBegin();
		//GWorld->OnBlit(); 
		Editor::GetEditor()->OnRender();
		GRenderer->RenderEnd();
		// Blit RenderTarget  End
	}
}

World* Engine::GetWorld() const
{
	return GWorld;
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

#ifdef _DEBUG
	LPCWSTR libFileName = L"Application_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"Application_x64_Release.dll";
#endif

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

#ifdef _DEBUG
	LPCWSTR libFileName = L"RendererD3D11_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"RendererD3D11_x64_Release.dll";
#endif
	
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


void Engine::CleanUp()
{
	Editor::GetEditor()->Release();

	if (nullptr != GParticle_1)
	{
		GParticle_1->Release();
		GParticle_1 = nullptr;
	}
	if (nullptr != GParticle_2)
	{
		GParticle_2->Release();
		GParticle_2 = nullptr;
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

	if (nullptr != GRasterizerManager)
	{
		delete GRasterizerManager;
		GRasterizerManager = nullptr;
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

	FBXManager::CleanUp();

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