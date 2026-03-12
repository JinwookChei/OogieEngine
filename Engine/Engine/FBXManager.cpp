#include "stdafx.h"
#include "FBXManager.h"

typedef bool (*DLL_FUNCTION_ARG1)(void**);

IFBXImporter* FBXManager::GFbxImporter = nullptr;
HMODULE FBXManager::GFbxImporterModule = nullptr;

bool FBXManager::LoadFBXImporter()
{
	FBXManager::ShutDown();

#ifdef _DEBUG
	LPCWSTR libFileName = L"FBXImporter_x64_Debug.dll";
#else
	LPCWSTR libFileName = L"FBXImporter_x64_Release.dll";

#endif

	FBXManager::GFbxImporterModule = LoadLibrary(libFileName);
	if (!FBXManager::GFbxImporterModule)
	{
		DEBUG_BREAK();
		FBXManager::ShutDown();
		return false;
	}

	DLL_FUNCTION_ARG1 CreateFbxImporter = (DLL_FUNCTION_ARG1)GetProcAddress(FBXManager::GFbxImporterModule, "CreateFBXImporter");
	if (!CreateFbxImporter)
	{
		DEBUG_BREAK();
		FBXManager::ShutDown();
		return false;
	}

	CreateFbxImporter((void**)&FBXManager::GFbxImporter);
	if (nullptr == FBXManager::GFbxImporter)
	{
		DEBUG_BREAK();
		FBXManager::ShutDown();
		return false;
	}

	return true;
}

void FBXManager::TestLoad()
{
	//Model wukong;
	//FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong.FBX");
	////FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong_Fury.FBX");
	//MeshDesc meshDesc_3;
	//meshDesc_3.primitiveType = E_MESH_PRIMITIVE_TYPE::Triangle;
	//meshDesc_3.resourceFlag = E_MESH_RESOURCE_FLAG::None;
	//meshDesc_3.vertexFormatSize = sizeof(SkinnedMeshVertex);
	//meshDesc_3.vertexCount = wukong.meshInfos[0].vertices.size();
	//meshDesc_3.pVertices = wukong.meshInfos[0].vertices.data();
	//meshDesc_3.meshSubsets.emplace_back(0, (uint16_t)sizeof(uint16_t), wukong.meshInfos[0].indices[0].size(), wukong.meshInfos[0].indices[0].data());
	//MeshManager::Instance()->CreateMesh(meshDesc_3, 12);
	//SkeletonManager::Instance()->CreateSkeleton(wukong.meshInfos[0].bones, 12);


	Model capoeira;
	FBXManager::LoadModel(&capoeira, "..\\Resource\\Fbx\\Mixamo\\Capoeira.FBX");
	MeshDesc meshDesc_4;
	meshDesc_4.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	meshDesc_4.usage = E_MESH_USAGE::DEFAULT;
	meshDesc_4.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	meshDesc_4.bufferSize = sizeof(SkinnedMeshVertex) * capoeira.meshInfos[0].vertices.size();
	meshDesc_4.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_4.vertexCount = capoeira.meshInfos[0].vertices.size();
	meshDesc_4.pVertices = capoeira.meshInfos[0].vertices.data();
	for (int i = 0; i < capoeira.meshInfos[0].indices.size(); ++i)
	{
		meshDesc_4.meshSubsets.emplace_back(i, (uint16_t)sizeof(uint16_t), capoeira.meshInfos[0].indices[i].size(), capoeira.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(meshDesc_4, 13);
	SkeletonManager::Instance()->CreateSkeleton(capoeira.meshInfos[0].bones, 13);


	Model wereWolf;
	FBXManager::LoadModel(&wereWolf, "..\\Resource\\Fbx\\WereWolf\\Model\\Werewolf_1.FBX");
	MeshDesc meshDesc_5;
	meshDesc_5.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	meshDesc_5.usage = E_MESH_USAGE::DEFAULT;
	meshDesc_5.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	meshDesc_5.bufferSize = sizeof(SkinnedMeshVertex) * wereWolf.meshInfos[0].vertices.size();
	meshDesc_5.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_5.vertexCount = wereWolf.meshInfos[0].vertices.size();
	meshDesc_5.pVertices = wereWolf.meshInfos[0].vertices.data();
	for (int i = 0; i < wereWolf.meshInfos[0].indices.size(); ++i)
	{
		meshDesc_5.meshSubsets.emplace_back(i, (uint16_t)sizeof(uint16_t), wereWolf.meshInfos[0].indices[i].size(), wereWolf.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(meshDesc_5, 14);
	SkeletonManager::Instance()->CreateSkeleton(wereWolf.meshInfos[0].bones, 14);
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

bool FBXManager::LoadAnimation(Animation* pOutAnimation, const std::string& file)
{
	if (nullptr == GFbxImporter)
	{
		DEBUG_BREAK();
		return false;
	}

	GFbxImporter->ImportAnimation(pOutAnimation, file, 1.0f / 60.0f);
	return true;
}


void FBXManager::ShutDown()
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