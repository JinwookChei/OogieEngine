#include "stdafx.h"
#include "StartUp.h"
#include "Engine.h"

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);
typedef bool (*DLL_FUNCTION_ARG1)(void**);

Engine::Engine()
	: startUp_(nullptr),
	application_(nullptr),
	renderer_(nullptr),
	applicationModule_(nullptr),
	rendererModule_(nullptr)
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

void Engine::DestroyEngine(Engine* engine)
{
	if (nullptr == engine)
	{
		DEBUG_BREAK();
		return;
	}

	delete engine;
}

bool Engine::Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow, IStartup* startup)
{
	if (nullptr == startup)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow))
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadRenderer())
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitializeStartUp(startup))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Engine::Run()
{
	while (false == application_->ApplicationQuit())
	{
		application_->WinPumpMessage();

		renderer_->BeginRender();

		// ... render

		renderer_->EndRender();
	}

	if (nullptr != startUp_)
	{
		startUp_->End();
		startUp_->Release();
		startUp_ = nullptr;
	}
}

bool Engine::LoadApplication(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	applicationModule_ = LoadLibrary(L"Application_x64_Debug.dll");
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

	const wchar_t* iConPath = L"..//..//Resource//Logo//LogoResize.ico";
	const wchar_t* MainWindowClassName = L"MAIN";
	const wchar_t* MainWindowText = L"WOOGIE ENGINE";

	CreateWindowsApplication((void**)&application_, hInstance, pCmdLine, nCmdShow, iConPath);
	if (nullptr == application_) 
	{
		DEBUG_BREAK();
		return false;
	}
		
	if (false == application_->InitializeMainWindow(MainWindowClassName, MainWindowText)) 
	{
		return false;
	}

	return true;
}

bool Engine::LoadRenderer()
{
	rendererModule_ = LoadLibrary(L"RendererD3D11_x64_Debug.dll");
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

	CreateRenderer((void**)&renderer_);
	if (nullptr == renderer_) 
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == renderer_->Initialize(application_->GetMainWindowHandle(), 800, 600))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

bool Engine::InitializeStartUp(IStartup* startUp)
{
	if (nullptr == startUp)
	{
		DEBUG_BREAK();
		return false;
	}

	startUp_ = startUp;
	startUp_->AddRef();

	if (false == startUp_->Start())
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Engine::CleanUp()
{
	if (nullptr != startUp_)
	{
		startUp_->Release();
		startUp_ = nullptr;
	}
	if (nullptr != renderer_)
	{
		renderer_->Release();
		renderer_ = nullptr;
	}
	if (nullptr != application_)
	{
		application_->Release();
		application_ = nullptr;
	}

	if (nullptr != rendererModule_)
	{
		FreeLibrary(rendererModule_);
		rendererModule_ = nullptr;
	}
	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}
}
