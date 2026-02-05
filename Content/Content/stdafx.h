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
#define EDITOR_API __declspec(dllimport)
//#define IMGUI_API __declspec(dllimport)

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
#include <vector>
#include <map>
#include <string>
#include <functional>

#include <DirectXMath.h>

// CoreTypes
#include <Common/MathType/MathType.h>
#include <Common/GeometryType/GeometryType.h>
#include <Common/VertexType/VertexType.h>


// Interface
#include <Interface/Application/IApplication.h>
#include <Interface/Renderer/IMesh.h>
#include <Interface/Renderer/ITexture.h>
#include <Interface/Renderer/IMaterial.h>
#include <Interface/Renderer/IRenderTarget.h>
#include <Interface/Renderer/IRasterizer.h>
#include <Interface/Renderer/IParticle.h>
#include <Interface/Renderer/IRenderResourceFactory.h>
#include <Interface/Renderer/IRenderer.h>
#include <Interface/EditorBindObject/IEditorBindObject.h>
#include <Interface/Editor/IEditor.h>


// Utility
#include <Utility/Utility/Math.h>
#include <Utility/Utility/LinkedList.h>
#include <Utility/Utility/HashTable.h>
 

// ImguiModule
#include <Editor/Editor/EditorCore.h>


// Engine
#include <Engine/Engine/Engine.h>
#include <Engine/Engine/StartUp.h>
#include <Engine/Engine/MeshManager.h>
#include <Engine/Engine/Transform.h>
#include <Engine/Engine/Level.h>
#include <Engine/Engine/Actor.h>
#include <Engine/Engine/Player.h>
#include <Engine/Engine/Camera.h>
#include <Engine/Engine/Light.h>
#include <Engine/Engine/DirectionLight.h>
#include <Engine/Engine/SpotLight.h>
#include <Engine/Engine/PointLight.h>
#include <Engine/Engine/RenderComponent.h>
#include <Engine/Engine/InputManager.h>
#include <Engine/Engine/World.h>

// Content
#include "StartLevel.h"
#include "ContentManager.h"

