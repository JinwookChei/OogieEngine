#include "stdafx.h"
#include "Engine.h"
#include "Application.h"

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);

Application* GApplication = nullptr;

Application::Application()
	:pApplicationImpl_(nullptr),
	applicationModule_(nullptr)
{
}

Application::~Application()
{
	CleanUp();
}

Application* Application::Create()
{
	if (nullptr != GApplication)
	{
		Application::Destroy();
	}

	GApplication = new Application;
	return GApplication;
}

void Application::Destroy()
{
	if (nullptr != GApplication)
	{
		delete GApplication;

		GApplication = nullptr;
	}
}

Application* Application::Instance()
{
	return GApplication;
}

bool Application::Load
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath,
	LPCWSTR libFileName
)
{
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

	CreateWindowsApplication((void**)&pApplicationImpl_, hInstance, pCmdLine, nCmdShow, pIConPath);
	if (nullptr == pApplicationImpl_)
	{
		DEBUG_BREAK();
		return false;
	}


	if (false == pApplicationImpl_->InitializeMainWindow(pMainWindowClassName, pMainWindowText))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

void Application::WinPumpMessage()
{
	if (nullptr == pApplicationImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pApplicationImpl_->WinPumpMessage();
}

bool Application::ApplicationQuit()
{
	if (nullptr == pApplicationImpl_)
	{
		DEBUG_BREAK();
		return false;
	}

	return pApplicationImpl_->ApplicationQuit();
}

void Application::SetShowCursor(bool show)
{
	if (nullptr == pApplicationImpl_)
	{
		DEBUG_BREAK();
		return;
	}

	pApplicationImpl_->SetShowCursor(show);
}

void* Application::GetMainWindowHandle()
{
	if (nullptr == pApplicationImpl_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pApplicationImpl_->GetMainWindowHandle();
}

void Application::CleanUp()
{
	if (nullptr != pApplicationImpl_)
	{
		pApplicationImpl_->Release();
		pApplicationImpl_ = nullptr;
	}

	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}
}