#include "stdafx.h"
#include "StartUp.h"
#include "Engine.h"

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);
typedef bool (*DLL_FUNCTION_ARG1)(void**);

InputManager* GInputManager = nullptr;

Engine::Engine()
	: startUp_(nullptr),
	application_(nullptr),
	renderer_(nullptr),
	applicationModule_(nullptr),
	rendererModule_(nullptr),
	mesh_(nullptr),
	vertexShader_(nullptr),
	pixelShader_(nullptr)
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

bool Engine::Initialize
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* mainWindowClassName,
	const wchar_t* mainWindowText,
	const wchar_t* iConPath,
	IStartup* startup
)
{
	if (nullptr == startup)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == LoadApplication(hInstance, pCmdLine, nCmdShow, mainWindowClassName, mainWindowText, iConPath))
	{
		return false;
	}

	if (false == LoadRenderer())
	{
		return false;
	}

	GInputManager = new InputManager;
	if (nullptr == GInputManager)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == InitializeStartUp(startup))
	{
		return false;
	}

	return true;
}

void Engine::Run()
{
	SimpleVertex vertices[] = {
{DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
{DirectX::XMFLOAT3(0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
{DirectX::XMFLOAT3(-0.5f, -0.5f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	bool ret = renderer_->CreateVertex(&vertices, sizeof(SimpleVertex), _countof(vertices), &mesh_);
	if (false == ret)
	{
		return;
	}

	ret = renderer_->CreateShader(L"VertexShader.cso", true, &vertexShader_);
	if (false == ret)
	{
		return;
	}

	ret = renderer_->CreateShader(L"PixelShader.cso", false, &pixelShader_);
	if (false == ret)
	{
		return;
	}
	
	ret = mesh_->AddInputLayout("POSITION", 0, 6, 0, false);
	if (false == ret)
	{
		return;
	}
	ret = mesh_->AddInputLayout("COLOR", 0, 2, 0, false);
	if (false == ret)
	{
		return;
	}

	ret = mesh_->CreateInputLayout(vertexShader_);
	if (false == ret)
	{
		return;
	}

	mesh_->Setting();
	vertexShader_->Setting();
	pixelShader_->Setting();


	while (false == application_->ApplicationQuit())
	{
		application_->WinPumpMessage();

		renderer_->BeginRender();

		// ... render
		// TEMP
		renderer_->Render();

		renderer_->EndRender();
	}

	if (nullptr != startUp_)
	{
		startUp_->End();
		startUp_->Release();
		startUp_ = nullptr;
	}
}

bool Engine::LoadApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* mainWindowClassName,
	const wchar_t* mainWindowText,
	const wchar_t* iConPath
)
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

	CreateWindowsApplication((void**)&application_, hInstance, pCmdLine, nCmdShow, iConPath);
	if (nullptr == application_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == application_->InitializeMainWindow(mainWindowClassName, mainWindowText))
	{
		DEBUG_BREAK();
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
	if (nullptr != pixelShader_)
	{
		pixelShader_->Release();
		pixelShader_ = nullptr;
	}
	if (nullptr != vertexShader_)
	{
		vertexShader_->Release();
		vertexShader_ = nullptr;
	}

	if (nullptr != mesh_)
	{
		mesh_->Release();
		mesh_ = nullptr;
	}



	if (nullptr != startUp_)
	{
		startUp_->Release();
		startUp_ = nullptr;
	}

	if (nullptr != GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
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
