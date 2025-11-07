#pragma once

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new (_NORMAL_BLOCK, __FILE__, __LINE__)
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


#define ENGINE_API __declspec(dllexport)
#define UTILITY_API __declspec(dllimport)
#define IMGUI_API __declspec(dllimport)

#include <windows.h>
#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>

#include <Utility/Math.h>
#include <Utility/LinkedList.h>
#include <Utility/HashTable.h>


#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include <ImguiModule/IImguiModule.h>

#include <ImguiModule/ImguiManager.h>

#include "InputManager.h"
#include "TimeManager.h"
#include "ConstantManager.h"

#include "Camera.h"
#include "Light.h"
#include "SpotLight.h"

extern IApplication* GApplication;
extern IRenderer* GRenderer;
extern InputManager* GInputManager;
extern TimeManager* GTimeManager;
extern ConstantManager* GConstantManager;
extern Camera* GMainCamera;
extern Camera* GCurrentCamera;
extern IBlendState* GBlendState;


// юс╫ц.
constexpr float DEFAULT_SCREEN_WIDTH = 2560.0f;
constexpr float DEFAULT_SCREEN_HEIGHT = 1440.0f;
