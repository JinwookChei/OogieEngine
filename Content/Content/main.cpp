#include "stdafx.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int cmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(352);
#endif  // _DEBUG

	const wchar_t* mainWindowClassName = L"MAIN";
	const wchar_t* mainWindowText = L"WOOGIE ENGINE";
	const wchar_t* iConPath = L"..//Resource//Logo//LogoResize.ico";

	ContentManager* pContentManager = new ContentManager;
	if (nullptr == pContentManager)
	{
		DEBUG_BREAK();
		return 0;
	}

	if (false == pContentManager->Initialize(hInstance, pCmdLine, cmdShow, mainWindowClassName, mainWindowText, iConPath))
	{
		goto lb_return;
	}

	pContentManager->GetEngine()->GetWorld()->ChangeLevel<StartLevel>();
	
	pContentManager->EngineLoop();


lb_return:
	pContentManager->Release();
	pContentManager = nullptr;

#ifdef _DEBUG
	_ASSERT(_CrtCheckMemory());
#endif  // _DEBUG
	return 0;
}