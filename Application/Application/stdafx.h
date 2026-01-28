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
#define EDITOR_API __declspec(dllimport)

#include <windows.h>
#include <stdint.h>
#include <windowsx.h>

// STL
#include <string>
#include <map>

//CoreTypes
#include <Common/MathType/MathType.h>
#include <Common/GeometryType/GeometryType.h>
#include <Common/VertexType/VertexType.h>


// Utility
#include <Utility/Utility/Math.h>
#include <Utility/Utility/LinkedList.h>


// Interface
#include <Interface/Application/IApplication.h>
#include <Interface/Renderer/IRenderer.h>
#include <Interface/EditorBindObject/IEditorBindObject.h>
#include <Interface/Editor/IEditor.h>


// Editor
#include <Editor/Editor/EditorCore.h>

extern class WindowsApplication* GApplication;