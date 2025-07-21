#include "stdafx.h"
#include "AppManager.h"

IEngineContainer* EngineContainer = nullptr;

AppManager::AppManager()
	:refCount_(1),
	engine_(nullptr)
{
	EngineContainer = this;
}

AppManager::~AppManager()
{
	EngineContainer = nullptr;
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

	if (false == engine_->Initialize(hInstance, pCmdLine, cmdShow, this))
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
