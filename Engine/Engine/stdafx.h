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
#define EDITOR_API __declspec(dllimport)


#include <windows.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <functional>

// Temp
#include <DirectXMath.h>

// Common
#include <Common/MathType/MathType.h>
#include <Common/GeometryType/GeometryType.h>
#include <Common/VertexType/VertexType.h>
#include <Common/Asset/Animation.h>
#include <Common/Asset/Model.h>


// Utility
#include <Utility/Utility/Math.h>
#include <Utility/Utility/LinkedList.h>
#include <Utility/Utility/HashTable.h>


// Interface
#include <Interface/Application/IApplication.h>
#include <Interface/Renderer/IMesh.h>
#include <Interface/Renderer/IShader.h>
#include <Interface/Renderer/ITexture.h>
#include <Interface/Renderer/IMaterial.h>
#include <Interface/Renderer/IRenderTarget.h>
#include <Interface/Renderer/IRasterizer.h>
#include <Interface/Renderer/IParticle.h>
#include <Interface/Renderer/IPSO.h>
#include <Interface/Renderer/IRenderResourceFactory.h>
#include <Interface/Renderer/IRenderer.h>
#include <Interface/EditorBindObject/IEditorBindObject.h>
#include <Interface/Editor/IEditor.h>
#include <Interface/FBXImporter/IFBXImporter.h>


// Editor
#include <Editor/Editor/EditorCore.h>


// Engine
#include "Application.h"
#include "Renderer.h"
#include "FBXManager.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "RasterizerManager.h"
#include "Transform.h"
#include "ActorPicker.h"
#include "World.h"
#include "Camera.h"
#include "Light.h"
#include "SpotLight.h"

//extern IApplication* GApplication;
//extern IRenderer* GRenderer;

extern MeshManager* GMeshManager;
//extern ShaderManager* GShaderManager;
extern MaterialManager* GMaterialManager;
extern TextureManager* GTextureManager;
extern RasterizerManager* GRasterizerManager;

extern World* GWorld;
extern Camera* GMainCamera;
extern Camera* GCurrentCamera;
extern ActorPicker* GActorPicker;


extern IParticle* GParticle_1;
extern IParticle* GParticle_2;

// юс╫ц.
constexpr float DEFAULT_SCREEN_WIDTH = 2560.0f;
constexpr float DEFAULT_SCREEN_HEIGHT = 1440.0f;
constexpr float DEFAULT_NEAR = 0.01f;
constexpr float DEFAULT_FAR = 100.0f;
constexpr float DEFAULT_FOV = 60.0f;