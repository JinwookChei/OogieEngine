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

#define EDITOR_API __declspec(dllexport)
#define UTILITY_API __declspec(dllimport)

// Win32
#include <windows.h>
#include <stdint.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <map>

//#include <DirectXMath.h>

// Imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Imgui2/imgui.h>
#include <Imgui2/imgui_impl_win32.h>
#include <Imgui2/imgui_impl_dx11.h>

#include <ImguiGizmo/ImGuizmo.h>
#include <ImguiGizmo/ImSequencer.h>
#include <ImguiGizmo/ImZoomSlider.h>
#include <ImguiGizmo/ImCurveEdit.h>
#include <ImguiGizmo/GraphEditor.h>

// Editor
#include "Label.h"


// CoreTypes
#include <MathType/MathType.h>
#include <GeometryType/GeometryType.h>
#include <VertexType/VertexType.h>


// Utility
#include <Utility/Math.h>
#include <Utility/LinkedList.h>


// Interface
#include <Application/IApplication.h>
#include <Renderer/IRenderer.h>
#include <EditorBindObject/IEditorBindObject.h>
#include <Editor/IEditor.h>
//#include <ImGuiBindObject/IImGuiBindObject.h>
//#include <ImGuiModule/IImGuiModule.h>



constexpr float DEFAULT_SCREEN_WIDTH = 2560.0f;
constexpr float DEFAULT_SCREEN_HEIGHT = 1440.0f;