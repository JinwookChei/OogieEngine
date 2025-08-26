#include "stdafx.h"
#include "Engine.h"
#include "Application.h"

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);

Application::Application()
	:pApplicationInterface_(nullptr),
	applicationModule_(nullptr)
{
}

Application::~Application()
{
	CleanUp();
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

	CreateWindowsApplication((void**)&pApplicationInterface_, hInstance, pCmdLine, nCmdShow, pIConPath);
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return false;
	}


	if (false == pApplicationInterface_->InitializeMainWindow(pMainWindowClassName, pMainWindowText))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

Application* Application::Instance()
{
	return nullptr;
}

void Application::WinPumpMessage()
{
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return;
	}

	pApplicationInterface_->WinPumpMessage();
}

bool Application::ApplicationQuit()
{
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return false;
	}

	return pApplicationInterface_->ApplicationQuit();
}

void Application::SetShowCursor(bool show)
{
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return;
	}

	pApplicationInterface_->SetShowCursor(show);
}

void* Application::GetMainWindowHandle()
{
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return pApplicationInterface_->GetMainWindowHandle();
}

void Application::CleanUp()
{
	if (nullptr != pApplicationInterface_)
	{
		pApplicationInterface_->Release();
		pApplicationInterface_ = nullptr;
	}

	if (nullptr != applicationModule_)
	{
		FreeLibrary(applicationModule_);
		applicationModule_ = nullptr;
	}
}