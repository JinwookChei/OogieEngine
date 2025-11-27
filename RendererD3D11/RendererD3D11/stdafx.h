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


#define UTILITY_API __declspec(dllimport)

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <windows.h>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <cstdio>

// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXTex/DirectXTex.h>
#include <DirectXTex/DirectXTex.inl>

// Utility
#include <Utility/Math.h>

// CoreTypes
#include <VertexType/VertexType.h>

// Interface
#include <Renderer/IRenderer.h>



// RendererD3D11
#include "D3D11Renderer.h"
#include "BaseShader.h"



extern Renderer* GRenderer;
extern IRenderTarget* GCurrentSetRenderTarget;

