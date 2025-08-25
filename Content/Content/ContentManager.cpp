#include "stdafx.h"
#include "ContentManager.h"

IEngineContainer* GEngineContainer = nullptr;

ContentManager::ContentManager()
	:refCount_(1),
	engine_(nullptr)
{
	GEngineContainer = this;
}

ContentManager::~ContentManager()
{
	GEngineContainer = nullptr;
}

HRESULT __stdcall ContentManager::QueryInterface(REFIID riid, void** ppvObject)
{
	return E_NOTIMPL;
}

ULONG __stdcall ContentManager::AddRef()
{
	++refCount_;
	return refCount_;
}

ULONG __stdcall ContentManager::Release()
{
	--refCount_;
	ULONG tmpRefCount = refCount_;
	if (0 == refCount_) {
		delete this;
	}

	return tmpRefCount;
}

bool __stdcall ContentManager::Start()
{
	return true;
}

void __stdcall ContentManager::End()
{
}

bool ContentManager::Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int cmdShow)
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

void ContentManager::EngineLoop()
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

Engine* ContentManager::GetEngine()
{
	return engine_;
}
