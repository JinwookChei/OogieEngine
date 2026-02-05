#include "stdafx.h"
#include "Renderer.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

HMODULE Renderer::GRendererModule = nullptr;
IRenderer* Renderer::GRenderer = nullptr;
IRenderResourceFactory* Renderer::GRenderResourceFactory = nullptr;

bool Renderer::LoadRenderer()
{
	if (nullptr == Application::GApplication)
	{
		DEBUG_BREAK();
		return false;
	}

	if (nullptr != GRenderer)
	{
		DEBUG_BREAK();
		GRenderer->Release();
		GRenderer = nullptr;
	}

#ifdef _DEBUG
	LPCWSTR libFileName = L"RendererD3D11_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"RendererD3D11_x64_Release.dll";
#endif

	GRendererModule = LoadLibrary(libFileName);
	if (!GRendererModule)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateRenderer = (DLL_FUNCTION_ARG1)GetProcAddress(GRendererModule, "CreateRenderer");
	if (!CreateRenderer)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateRenderer((void**)&GRenderer);
	if (nullptr == GRenderer)
	{
		DEBUG_BREAK();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateRenderResourceFactory = (DLL_FUNCTION_ARG1)GetProcAddress(GRendererModule, "CreateRenderResourceFactory");
	if (!CreateRenderResourceFactory)
	{
		DEBUG_BREAK();
		return false;
	}

	CreateRenderResourceFactory((void**)&GRenderResourceFactory);
	if (nullptr == GRenderResourceFactory)
	{
		DEBUG_BREAK();
		return false;
	}

	void* pMainHwnd = Application::GApplication->GetMainWindowHandle();
	if (false == GRenderer->Initialize(pMainHwnd, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT))
	{
		DEBUG_BREAK();
		return false;
	}

	return true;
}

IRenderer* Renderer::Instance()
{
	return GRenderer;
}

IRenderResourceFactory* Renderer::GetFactory()
{
	return GRenderResourceFactory;
}

void Renderer::ShutDown()
{
	if (nullptr != GRenderer)
	{
		GRenderer->Release();
		GRenderer = nullptr;
	}
	if (nullptr != GRenderResourceFactory)
	{
		GRenderResourceFactory->Release();
		GRenderResourceFactory = nullptr;
	}
	if (nullptr != GRendererModule)
	{
		FreeLibrary(GRendererModule);
		GRendererModule = nullptr;
	}
}