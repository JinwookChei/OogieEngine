#include "stdafx.h"
#include "ContentManager.h"

IEngineContainer* GEngineContainer = nullptr;

ContentManager::ContentManager()
	:refCount_(1),
	pEngine_(nullptr)
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

bool ContentManager::Initialize
(
	HINSTANCE hInstance, 
	PWSTR pCmdLine, 
	int cmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath
)
{
	pEngine_ = Engine::CreateEngine();

	if (nullptr == pEngine_)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == pEngine_->Initialize(hInstance, pCmdLine, cmdShow, pMainWindowClassName, pMainWindowText, pIConPath, this))
	{
		Engine::DestroyEngine(pEngine_);
		return false;
	}

	return true;
}

void ContentManager::EngineLoop()
{
	if (nullptr == pEngine_)
	{
		DEBUG_BREAK();
		return;
	}

	pEngine_->Run();

	Engine::DestroyEngine(pEngine_);

	pEngine_ = nullptr;
}

Engine* ContentManager::GetEngine()
{
	return pEngine_;
}
