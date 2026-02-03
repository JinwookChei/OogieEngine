#include "stdafx.h"
#include "FBXManager.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

IFBXImporter* FBXManager::GFbxImporter = nullptr;
HMODULE FBXManager::GFbxImporterModule = nullptr;

bool FBXManager::Init()
{
	return LoadFBXImporter();
}

bool FBXManager::LoadFBXImporter()
{
	FBXManager::CleanUp();

#ifdef _DEBUG
	LPCWSTR libFileName = L"FBXImporter_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"FBXImporter_x64_Release.dll";

#endif

	FBXManager::GFbxImporterModule = LoadLibrary(libFileName);
	if (!FBXManager::GFbxImporterModule)
	{
		DEBUG_BREAK();
		FBXManager::CleanUp();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateFbxImporter = (DLL_FUNCTION_ARG1)GetProcAddress(FBXManager::GFbxImporterModule, "CreateFBXImporter");
	if (!CreateFbxImporter)
	{
		DEBUG_BREAK();
		FBXManager::CleanUp();
		return false;
	}

	CreateFbxImporter((void**)&FBXManager::GFbxImporter);
	if (nullptr == FBXManager::GFbxImporter)
	{
		DEBUG_BREAK();
		FBXManager::CleanUp();
		return false;
	}

	return true;
}

bool FBXManager::LoadModel(Model* pOutModel, const std::string& file)
{
	if (nullptr == GFbxImporter)
	{
		DEBUG_BREAK();
		return false;
	}

	GFbxImporter->ImportModel(pOutModel, file);
	return true;
}

//void FBXImporter::TestImportFBX()
//{
//	Model* pModel = new Model;
//	if (false == FBXImporter::GetImporter()->ImportModel(pModel, "..\\Resource\\Fbx\\Wukong\\Wukong.FBX"))
//	{
//		DEBUG_BREAK();
//		return;
//	}
//}


void FBXManager::CleanUp()
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