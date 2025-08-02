#include "stdafx.h"
#include "AppManager.h"

IEngineContainer* GEngineContainer = nullptr;

AppManager::AppManager()
	:refCount_(1),
	engine_(nullptr)
{
	GEngineContainer = this;
}

AppManager::~AppManager()
{
	GEngineContainer = nullptr;
}

HRESULT __stdcall AppManager::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall AppManager::AddRef()
{
	++refCount_;
	return refCount_;
}

ULONG __stdcall AppManager::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool __stdcall AppManager::Start()
{
	return true;
}

void __stdcall AppManager::End()
{
}

bool AppManager::Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int cmdShow)
{
	engine_ = Engine::CreateEngine();

	if (nullptr == engine_)
	{
		DEBUG_BREAK();
		return false;
	}

	const wchar_t* mainWindowClassName = L"MAIN";
	const wchar_t* mainWindowText = L"WOOGIE ENGINE";
	const wchar_t* iConPath = L"..//Resource//Logo//LogoResize.ico";

	if (false == engine_->Initialize(hInstance, pCmdLine, cmdShow, mainWindowClassName, mainWindowText, iConPath, this))
	{
		Engine::DestroyEngine(engine_);
		return false;
	}

	return true;
}

void AppManager::EngineLoop()
{
	if (nullptr == engine_)
	{
		DEBUG_BREAK();
		return;
	}

	engine_->Run();

	Engine::DestroyEngine(engine_);

	engine_ = nullptr;
}

Engine* AppManager::GetEngine()
{
	return engine_;
}
