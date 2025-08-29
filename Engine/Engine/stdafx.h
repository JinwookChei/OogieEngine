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

#define ENGINE_API __declspec(dllexport)
#define UTILITY_API __declspec(dllimport)

#include <windows.h>
// TEST용 Engine에서는 DirextX 라이브러리 가지지 X
#include <DirectXMath.h>
#include <vector>

#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>

#include <Utility/Float4.h>
#include <Utility/LinkedList.h>
#include <Utility/HashTable.h>

#include "Application.h"
#include "RenderDevice.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Camera.h"

extern Application* GApplication;
extern RenderDevice* GRenderDevice;
extern InputManager* GInputManager;
extern TimeManager* GTimeManager;
extern Camera* GCamera;


#define PI 3.141592654f

