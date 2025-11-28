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

#define UTILITY_API __declspec(dllexport)

#define Assert(msg)                                                   \
    do {                                                               \
        char buffer[1024];                                             \
        std::snprintf(buffer, sizeof(buffer),                           \
            "Assertion failed: %s\nFile: %s\nLine: %d",               \
            msg, __FILE__, __LINE__);                                  \
        MessageBoxA(nullptr, buffer, "Assertion Failed", MB_ICONERROR);\
        std::abort();                                                  \
    } while(0)


#include <Windows.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <DirectXMath.h>


// CoreTypes
#include <MathType/MathType.h>
#include <GeometryType/GeometryType.h>


// Utility
#include "LinkedList.h"
#include "HashTable.h"
#include "Math.h"



