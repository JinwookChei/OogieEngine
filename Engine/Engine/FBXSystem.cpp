#include "stdafx.h"
#include "FBXSystem.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

IFBXImporter* FBXSystem::GFbxImporter = nullptr;
HMODULE FBXSystem::GFbxImporterModule = nullptr;

bool FBXSystem::Init()
{
	return LoadFBXImporter();
}

bool FBXSystem::LoadFBXImporter()
{
	FBXSystem::CleanUp();

#ifdef _DEBUG
	LPCWSTR libFileName = L"FBXImporter_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"FBXImporter_x64_Release.dll";

#endif

	FBXSystem::GFbxImporterModule = LoadLibrary(libFileName);
	if (!FBXSystem::GFbxImporterModule)
	{
		DEBUG_BREAK();
		FBXSystem::CleanUp();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateFbxImporter = (DLL_FUNCTION_ARG1)GetProcAddress(FBXSystem::GFbxImporterModule, "CreateFBXImporter");
	if (!CreateFbxImporter)
	{
		DEBUG_BREAK();
		FBXSystem::CleanUp();
		return false;
	}

	CreateFbxImporter((void**)&FBXSystem::GFbxImporter);
	if (nullptr == FBXSystem::GFbxImporter)
	{
		DEBUG_BREAK();
		FBXSystem::CleanUp();
		return false;
	}

	return true;
}

IFBXImporter* FBXSystem::GetImporter()
{
	if (nullptr == GFbxImporter)
	{
		DEBUG_BREAK();
		return nullptr;
	}

	return GFbxImporter;
}

void FBXSystem::CleanUp()
{
	if (nullptr != GFbxImporter)
	{
		GFbxImporter->Release();
		GFbxImporter = nullptr;
	}

	if (nullptr != GFbxImporterModule)
	{
		FreeLibrary(GFbxImporterModule);
		GFbxImporterModule = nullptr;
	}
}