#include "stdafx.h"
#include "Engine.h"
#include "Application.h"

Application::Application()
	:pApplicationInterface_(nullptr),
	applicationModule_(nullptr)
{
}

Application::~Application()
{
	CleanUp();
}

Application* Application::Instance()
{
	return nullptr;
}

bool Application::InitializeMainWindow(const wchar_t* className, const wchar_t* windowText)
{
	if (nullptr == pApplicationInterface_)
	{
		DEBUG_BREAK();
		return false;
	}

	return pApplicationInterface_->InitializeMainWindow(className, windowText);
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

void Application::SetApplicationInterface(IApplication* pApplication)
{
	if (nullptr == pApplication)
	{
		DEBUG_BREAK();
		return;
	}

	pApplicationInterface_ = pApplication;
}

void Application::SetApplicationModule(HMODULE appModule)
{
	if (!appModule)
	{
		DEBUG_BREAK();
		return;
	}

	applicationModule_ = appModule;
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