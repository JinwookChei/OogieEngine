#include "stdafx.h"
#include "WindowsApplication.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        //_CrtSetBreakAlloc(352);
#endif  // _DEBUG
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
#ifdef _DEBUG
        _ASSERT(_CrtCheckMemory());
#endif  // _DEBUG
        break;
    }
    return TRUE;
}


extern "C" __declspec(dllexport) bool __stdcall CreateWindowsApplication(
    void** newApplication,
    HINSTANCE hInstance,
    PWSTR pCmdLine,
    int nCmdShow,
    const wchar_t* iconPath)
{
    //NEW -> Release In GameMain.cpp
    IApplication* newApp = new WindowsApplication(hInstance, pCmdLine, nCmdShow, iconPath);

    if (nullptr == newApp)
    {
        DEBUG_BREAK();
        return false;
    }

    *newApplication = newApp;

    return true;
}
