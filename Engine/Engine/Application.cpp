#include "stdafx.h"
#include "Application.h"

typedef bool (*DLL_FUNCTION_ARG5)(void**, HINSTANCE, PWSTR, int, const wchar_t*);

HMODULE Application::GApplicationModule = nullptr;
IApplication* Application::GApplication = nullptr;


bool Application::LoadApplication
(
	HINSTANCE hInstance,
	PWSTR pCmdLine,
	int nCmdShow,
	const wchar_t* pMainWindowClassName,
	const wchar_t* pMainWindowText,
	const wchar_t* pIConPath
)
{
	if (nullptr != GApplication)
	{
		DEBUG_BREAK();
		GApplication->Release();
		GApplication = nullptr;
	}

#ifdef _DEBUG
	LPCWSTR libFileName = L"Application_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"Application_x64_Release.dll";
#endif

	GApplicationModule = LoadLibrary(libFileName);
	if (!GApplicationModule)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG5 CreateWindowsApplication = (DLL_FUNCTION_ARG5)GetProcAddress(GApplicationModule, "CreateWindowsApplication");
	if (!CreateWindowsApplication)
	{
		DEBUG_BREAK();
		return false;
	}


	CreateWindowsApplication
	(
		(void**)&GApplication,
		hInstance,
		pCmdLine,
		nCmdShow,
		pIConPath
	);

	if (nullptr == GApplication)
	{
		DEBUG_BREAK();
		return false;
	}

	if (false == GApplication->InitializeMainWindow(pMainWindowClassName, pMainWindowText, { DEFAULT_SCREEN_WIDTH , DEFAULT_SCREEN_HEIGHT }))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

IApplication* Application::Instance()
{
	return GApplication;
}

void Application::ShutDown()
{
	if (nullptr != GApplication)
	{
		GApplication->Release();
		GApplication = nullptr;
	}

	if (nullptr != GApplicationModule)
	{
		FreeLibrary(GApplicationModule);
		GApplicationModule = nullptr;
	}
}