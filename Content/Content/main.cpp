#include "stdafx.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int cmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(352);
#endif  // _DEBUG

	AppManager* appManager = new AppManager;

	if (false == appManager->Initialize(hInstance, pCmdLine, cmdShow))
	{
		goto lb_return;
	}

	appManager->EngineLoop();

lb_return:
	appManager->Release();
	appManager = nullptr;

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif  // _DEBUG
	return 0;
}