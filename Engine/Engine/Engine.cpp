#include "stdafx.h"
#include "StartUp.h"
#include "Level.h"
#include "World.h"
#include "Engine.h"
#include "Actor.h"

MeshManager* GMeshManager = nullptr;
//ShaderManager* GShaderManager = nullptr;
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

	GMeshManager = new MeshManager;

	//GShaderManager = new ShaderManager;

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
	TextureManager::Instance()->TestLoad();
	MaterialManager::Instance()->TestLoad();

	ParticleDesc particleDesc;
	particleDesc.maxNum_ = 10;
	particleDesc.patternType_ = E_PARTICLE_PATTERN_TYPE::EXPLOSION;
	TextureManager::Instance()->GetTexture(&particleDesc.pTexture_, 3);
	GParticle_1 = Renderer::GetFactory()->CreateParticle(particleDesc);

	ParticleDesc particleDesc2;
	particleDesc2.maxNum_ = 10;
	particleDesc2.patternType_ = E_PARTICLE_PATTERN_TYPE::FUME;
	TextureManager::Instance()->GetTexture(&particleDesc2.pTexture_, 3);
	GParticle_2 = Renderer::GetFactory()->CreateParticle(particleDesc2);


	while (false == Application::Instance()->ApplicationQuit()) {

		Application::Instance()->WinPumpMessage();

		// Calc DeltaTime
		double deltaTime = TimeManager::CalcDeltaTime();

		// Input Update
		InputManager::Tick(deltaTime);
		
		// 잠시 에러 있어서 OFF
		//GActorPicker->Tick(deltaTime);

		Renderer::Instance()->UpdateParticles(GParticle_1, deltaTime);
		Renderer::Instance()->UpdateParticles(GParticle_2, deltaTime);
		
		// GameLoop
		GWorld->CheckChangeLevel();
		GWorld->OnTick(deltaTime);
		GWorld->OnRender();
		// GameLoop End

		// Blit RenderTarget 
		Renderer::Instance()->RenderBegin();
		//GWorld->OnBlit(); 
		Editor::GetEditor()->OnRender();
		Renderer::Instance()->RenderEnd();
		// Blit RenderTarget  End
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
	//if (nullptr != GShaderManager)
	//{
	//	delete GShaderManager;
	//	GShaderManager = nullptr;
	//}
	if (nullptr != GMeshManager)
	{
		delete GMeshManager;
		GMeshManager = nullptr;
	}

	TimeManager::CleanUp();
	
	InputManager::CleanUp();

	FBXManager::ShutDown();
	Renderer::ShutDown();
	Application::ShutDown();
}