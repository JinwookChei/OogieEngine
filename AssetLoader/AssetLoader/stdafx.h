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



#include <Windows.h>


//#include <fbxsdk.h>
// FBX
#pragma comment(lib, "alembic-md")
#pragma comment(lib, "libfbxsdk-md")
#pragma comment(lib, "libxml2-md")
#pragma comment(lib, "zlib-md")