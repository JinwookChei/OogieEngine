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

#define UTILITY_API __declspec(dllimport)


#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <vector>

#include <Renderer/IRenderer.h>
#include <Utility/Float4.h>

#include "D3D11Renderer.h"
#include "BaseShader.h"

extern D3D11Renderer* GRenderer;

//void DumpLiveD3D11Objects();

