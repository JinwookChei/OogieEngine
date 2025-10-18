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
#define UTILITY_API __declspec(dllimport)


#ifdef _DEBUG
#define DEBUG_BREAK __debugbreak
#else
#define DEBUG_BREAK
#endif // DEBUG


#define Assert(msg)                                                   \
    do {                                                               \
        char buffer[1024];                                             \
        std::snprintf(buffer, sizeof(buffer),                           \
            "Assertion failed: %s\nFile: %s\nLine: %d",               \
            msg, __FILE__, __LINE__);                                  \
        MessageBoxA(nullptr, buffer, "Assertion Failed", MB_ICONERROR);\
        std::abort();                                                  \
    } while(0)


#ifndef UNICODE
#define UNICODE
#endif 


#include <windows.h>
#include <cstdint>
#include <cstdlib>
#include <cstdio>

#include <Utility/Math.h>
#include <Utility/LinkedList.h>
#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include <Engine/Engine.h>
#include <Engine/StartUp.h>
#include <Engine/Level.h>
#include <Engine/Actor.h>
#include <Engine/Camera.h>
#include <Engine/RenderComponent.h>
#include <Engine/InputManager.h>


#include <Engine/World.h>
#include "StartLevel.h"

#include "ContentManager.h"

