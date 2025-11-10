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


#ifndef UNICODE
#define UNICODE
#endif 

#define UTILITY_API __declspec(dllexport)
#define IMGUI_API __declspec(dllimport)

#include <windows.h>
#include <stdint.h>
#include <windowsx.h>

// Utility
#include <Utility\Math.h>
#include <Utility\LinkedList.h>

// Interface
#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include <ImGuiBindObject/IImGuiBindObject.h>
#include <ImGuiModule/IImGuiModule.h>

// ImguiManager
#include <ImGuiModule/ImGuiManager.h>

extern class WindowsApplication* GApplication;