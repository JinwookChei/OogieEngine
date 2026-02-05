#include "stdafx.h"


#ifdef _DEBUG
void CheckMemoryLeaks_D3DObjects()
{
	HMODULE dxgiDebugDll = GetModuleHandleW(L"dxgidebug.dll");
	if (!dxgiDebugDll)
	{
		OutputDebugStringW(L"dxgidebug.dll not loaded.\n");
		return;
	}

	auto dxgiGetDebugInterface = reinterpret_cast<HRESULT(WINAPI*)(REFIID, void**)>(GetProcAddress(dxgiDebugDll, "DXGIGetDebugInterface"));

	if (!dxgiGetDebugInterface)
	{
		OutputDebugStringW(L"DXGIGetDebugInterface not found.\n");
		return;
	}

	IDXGIDebug* pDebug = nullptr;
	HRESULT hr = dxgiGetDebugInterface(IID_PPV_ARGS(&pDebug));
	if (FAILED(hr) || !pDebug)
	{
		OutputDebugStringW(L"Failed to get IDXGIDebug interface.\n");
		return;
	}
	
	OutputDebugStringW(L"¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å¡å\r\n");
	OutputDebugStringW(L"Starting Live Direct3D Object Dump:\r\n");
	pDebug->ReportLiveObjects(DXGI_DEBUG_D3D11, DXGI_DEBUG_RLO_DETAIL);
	OutputDebugStringW(L"Completed Live Direct3D Object Dump.\r\n");
	OutputDebugStringW(L"¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã¡ã\r\n");

	// ¹Ýµå½Ã Release
	pDebug->Release();
}
#endif

// Renderer
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(352);
#endif  // _DEBUG
	} break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
	{
#ifdef _DEBUG
		CheckMemoryLeaks_D3DObjects();
		_ASSERT(_CrtCheckMemory());
#endif  // _DEBUG
	} break;
	}
	return TRUE;
}


extern "C" __declspec(dllexport) bool CreateRenderer(void** pVoid)
{
	IRenderer* pNew = new Renderer;
	if (nullptr == pNew)
	{
		DEBUG_BREAK();
		return false;
	}

	*pVoid = pNew;
	return true;
}


extern "C" __declspec(dllexport) bool CreateRenderResourceFactory(void** pVoid)
{
	IRenderResourceFactory* pNew = new RenderResourceFactory;
	if (nullptr == pNew)
	{
		DEBUG_BREAK();
		return false;
	}

	*pVoid = pNew;
	return true;
}