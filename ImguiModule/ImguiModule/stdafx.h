#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
//#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif  // _DEBUG

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

#define IMGUI_API __declspec(dllexport)
#define UTILITY_API __declspec(dllimport)

// Win32
#include <windows.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>


// Imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Imgui/imgui.h>
#include <Imgui/imgui_impl_win32.h>
#include <Imgui/imgui_impl_dx11.h>

// Utility
#include <Utility/Math.h>
#include <Utility/LinkedList.h>

// Interface
#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include <ImguiBindObject/IImguiBindObject.h>
#include <ImguiModule/IImguiModule.h>


#include "ImguiPanel.h"
#include "ImguiWidget.h"
