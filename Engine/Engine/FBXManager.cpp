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
	StaticModel lightBulb;
	FBXManager::LoadStaticModel(&lightBulb, "..\\Resource\\Fbx\\LightBulb\\SM_Lamp7.FBX");
	MeshDesc lightBulbMeshDesc;
	lightBulbMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	lightBulbMeshDesc.vertexType = E_VERTEX_TYPE::STATIC_MESH;
	lightBulbMeshDesc.usage = E_MESH_USAGE::DEFAULT;
	lightBulbMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	lightBulbMeshDesc.bufferSize = sizeof(StaticMeshVertex) * lightBulb.meshInfos[0].vertices.size();
	lightBulbMeshDesc.vertexFormatSize = sizeof(StaticMeshVertex);
	lightBulbMeshDesc.vertexCount = lightBulb.meshInfos[0].vertices.size();
	lightBulbMeshDesc.pVertices = lightBulb.meshInfos[0].vertices.data();
	for (int i = 0; i < lightBulb.meshInfos[0].indices.size(); ++i)
	{
		lightBulbMeshDesc.subMeshDesc.emplace_back(i, (uint32_t)lightBulb.meshInfos[0].indices[i].size(), lightBulb.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(lightBulbMeshDesc, "MS_LightBulb", 12);

	SkeletalModel capoeira;
	FBXManager::LoadSkeletalModel(&capoeira, "..\\Resource\\Fbx\\Mixamo\\Capoeira.FBX");
	MeshDesc capoeiraMeshDesc;
	capoeiraMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	capoeiraMeshDesc.vertexType = E_VERTEX_TYPE::SKINNED_MESH;
	capoeiraMeshDesc.usage = E_MESH_USAGE::DEFAULT;
	capoeiraMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	capoeiraMeshDesc.bufferSize = sizeof(SkinnedMeshVertex) * capoeira.meshInfos[0].vertices.size();
	capoeiraMeshDesc.vertexFormatSize = sizeof(SkinnedMeshVertex);
	capoeiraMeshDesc.vertexCount = capoeira.meshInfos[0].vertices.size();
	capoeiraMeshDesc.pVertices = capoeira.meshInfos[0].vertices.data();
	for (int i = 0; i < capoeira.meshInfos[0].indices.size(); ++i)
	{
		capoeiraMeshDesc.subMeshDesc.emplace_back(i, (uint32_t)capoeira.meshInfos[0].indices[i].size(), capoeira.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(capoeiraMeshDesc, "MS_Capoeira", 11);
	SkeletonManager::Instance()->CreateSkeleton(capoeira.meshInfos[0].bones, "SK_Campeira", 11);

	SkeletalModel werewolf;
	FBXManager::LoadSkeletalModel(&werewolf, "..\\Resource\\Fbx\\WereWolf\\Model\\Werewolf_1.FBX");
	MeshDesc werewolfMeshDesc;
	werewolfMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	werewolfMeshDesc.vertexType = E_VERTEX_TYPE::SKINNED_MESH;
	werewolfMeshDesc.usage = E_MESH_USAGE::DEFAULT;
	werewolfMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	werewolfMeshDesc.bufferSize = sizeof(SkinnedMeshVertex) * werewolf.meshInfos[0].vertices.size();
	werewolfMeshDesc.vertexFormatSize = sizeof(SkinnedMeshVertex);
	werewolfMeshDesc.vertexCount = werewolf.meshInfos[0].vertices.size();
	werewolfMeshDesc.pVertices = werewolf.meshInfos[0].vertices.data();
	for (int i = 0; i < werewolf.meshInfos[0].indices.size(); ++i)
	{
		werewolfMeshDesc.subMeshDesc.emplace_back(i, (uint32_t)werewolf.meshInfos[0].indices[i].size(), werewolf.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(werewolfMeshDesc, "MS_Werewolf", 11);
	SkeletonManager::Instance()->CreateSkeleton(werewolf.meshInfos[0].bones, "SK_Werewolf", 11);

	StaticModel house;
	FBXManager::LoadStaticModel(&house, "..\\Resource\\Fbx\\House\\SM_House_Two.FBX");
	MeshDesc houseMeshDesc;
	houseMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	houseMeshDesc.vertexType = E_VERTEX_TYPE::STATIC_MESH;
	houseMeshDesc.usage = E_MESH_USAGE::DEFAULT;
	houseMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	houseMeshDesc.bufferSize = sizeof(StaticMeshVertex) * house.meshInfos[0].vertices.size();
	houseMeshDesc.vertexFormatSize = sizeof(StaticMeshVertex);
	houseMeshDesc.vertexCount = house.meshInfos[0].vertices.size();
	houseMeshDesc.pVertices = house.meshInfos[0].vertices.data();
	for (int i = 0; i < house.meshInfos[0].indices.size(); ++i)
	{
		houseMeshDesc.subMeshDesc.emplace_back(i, (uint32_t)house.meshInfos[0].indices[i].size(), house.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(houseMeshDesc, "MS_House", 8);

	StaticModel tree;
	FBXManager::LoadStaticModel(&tree, "..\\Resource\\Fbx\\Tree\\SM_BlackAlder.FBX");
	MeshDesc treeMeshDesc;
	treeMeshDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	treeMeshDesc.vertexType = E_VERTEX_TYPE::STATIC_MESH;
	treeMeshDesc.usage = E_MESH_USAGE::DEFAULT;
	treeMeshDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	treeMeshDesc.bufferSize = sizeof(StaticMeshVertex) * tree.meshInfos[0].vertices.size();
	treeMeshDesc.vertexFormatSize = sizeof(StaticMeshVertex);
	treeMeshDesc.vertexCount = tree.meshInfos[0].vertices.size();
	treeMeshDesc.pVertices = tree.meshInfos[0].vertices.data();
	for (int i = 0; i < tree.meshInfos[0].indices.size(); ++i)
	{
		treeMeshDesc.subMeshDesc.emplace_back(i, (uint32_t)tree.meshInfos[0].indices[i].size(), tree.meshInfos[0].indices[i].data());
	}
	MeshManager::Instance()->CreateMesh(treeMeshDesc, "MS_Tree", 7);
}

bool FBXManager::LoadStaticModel(StaticModel* pOutModel, const std::string& file)
{
	if (nullptr == GFbxImporter)
	{
		DEBUG_BREAK();
		return false;
	}

	GFbxImporter->ImportStaticModel(pOutModel, file);
	return true;
}

bool FBXManager::LoadSkeletalModel(SkeletalModel* pOutModel, const std::string& file)
{
	if (nullptr == GFbxImporter)
	{
		DEBUG_BREAK();
		return false;
	}

	GFbxImporter->ImportSkeletalModel(pOutModel, file);
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