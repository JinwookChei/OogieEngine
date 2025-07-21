#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif  // _DEBUG

#ifdef _DEBUG
#ifdef _WIN64
#define APPLICATION_DLL L"Application_x64_Debug"
#else
#define APPLICATION_DLL L"Application_x86_Debug"
#endif // 
#else
#ifdef _WIN64
#define APPLICATION_DLL L"Application_x64_Release"
#else
#define APPLICATION_DLL L"Application_x86_Release"
#endif
#endif // _DEBUG

#define ENGINE_API __declspec(dllimport)


#ifdef _DEBUG
#define DEBUG_BREAK __debugbreak
#else
#define DEBUG_BREAK
#endif // DEBUG

#ifndef UNICODE
#define UNICODE
#endif 
#include <windows.h>

#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include "Engine.h"
#include "StartUp.h"

//#include "..\..\Engine\Engine\Engine.h"
//#include "..\..\Engine\Engine\StartUp.h"



#include "AppManager.h"
extern IEngineContainer* EngineContainer;