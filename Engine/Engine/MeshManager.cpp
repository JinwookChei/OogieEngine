#include "stdafx.h"
#include "MeshManager.h"
#include "GeometryGenerator.h"

MeshManager::MeshManager()
	: meshTable_(16,8)
{
}

MeshManager::~MeshManager()
{
	CleanUp();
}

MeshManager* MeshManager::Instance()
{
	return GMeshManager;
}

void MeshManager::TestLoad()	
{
	std::vector<ScreenQuadVertex> screenQuadVertices;
	std::vector<WORD> screenQuadIndices;
	GeometryGenerator::CreateScreenQuad(&screenQuadVertices, &screenQuadIndices);
	MeshDesc screenQuadDesc;
	screenQuadDesc.vertexFormat = E_VERTEX_FORMAT::ScreenQuad;
	screenQuadDesc.vertexFormatSize = sizeof(ScreenQuadVertex);
	screenQuadDesc.vertexCount = screenQuadVertices.size();
	screenQuadDesc.pVertices = screenQuadVertices.data();
	screenQuadDesc.meshSubsets.emplace_back(0, sizeof(WORD), screenQuadIndices.size(), screenQuadIndices.data());
	CreateMesh(screenQuadDesc, 0);


	std::vector<SimpleVertex> cubeVertices;
	std::vector<WORD> cubeIndices;
	GeometryGenerator::CreateCube(&cubeVertices, &cubeIndices);
	MeshDesc meshDesc_1;
	meshDesc_1.vertexFormat = E_VERTEX_FORMAT::Simple;
	meshDesc_1.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_1.vertexCount = cubeVertices.size();
	meshDesc_1.pVertices = cubeVertices.data();
	meshDesc_1.meshSubsets.emplace_back(0, sizeof(WORD), cubeIndices.size(),cubeIndices.data());
	unsigned long long meshTag_1 = 1;
	CreateMesh(meshDesc_1, meshTag_1);


	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;
	GeometryGenerator::CreateSphere(&sphereVertices, &sphereIndices);
	MeshDesc meshDesc_2;
	meshDesc_2.vertexFormat = E_VERTEX_FORMAT::Simple;
	meshDesc_2.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_2.vertexCount = sphereVertices.size();
	meshDesc_2.pVertices = sphereVertices.data();
	meshDesc_2.meshSubsets.emplace_back(0, sizeof(WORD), sphereIndices.size(), sphereIndices.data());
	unsigned long long meshTag_2 = 2;
	CreateMesh(meshDesc_2, meshTag_2);


	Model wukong;
	FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong.FBX");
	//FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong_Fury.FBX");
	MeshDesc meshDesc_3;
	meshDesc_3.vertexFormat = E_VERTEX_FORMAT::SkinnedMesh;
	meshDesc_3.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_3.vertexCount = wukong.meshInfo[0].vertices.size();
	meshDesc_3.pVertices = wukong.meshInfo[0].vertices.data();
	meshDesc_3.meshSubsets.emplace_back(0, sizeof(uint16_t), wukong.meshInfo[0].indices[0].size(), wukong.meshInfo[0].indices[0].data());
	
	unsigned long long meshTag_3 = 3;
	CreateMesh(meshDesc_3, meshTag_3);


	Model capoeira;
	FBXManager::LoadModel(&capoeira, "..\\Resource\\Fbx\\Mixamo\\Capoeira.FBX");
	MeshDesc meshDesc_4;
	meshDesc_4.vertexFormat = E_VERTEX_FORMAT::SkinnedMesh;
	meshDesc_4.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_4.vertexCount = capoeira.meshInfo[0].vertices.size();
	meshDesc_4.pVertices = capoeira.meshInfo[0].vertices.data();
	for (int i = 0; i < capoeira.meshInfo[0].indices.size(); ++i)
	{
		meshDesc_4.meshSubsets.emplace_back(i, sizeof(uint16_t), capoeira.meshInfo[0].indices[i].size(), capoeira.meshInfo[0].indices[i].data());
	}
	unsigned long long meshTag_4 = 4;
	CreateMesh(meshDesc_4, meshTag_4);


	Model wereWolf;
	FBXManager::LoadModel(&wereWolf, "..\\Resource\\Fbx\\WereWolf\\Model\\Werewolf_1.FBX");
	MeshDesc meshDesc_5;
	meshDesc_5.vertexFormat = E_VERTEX_FORMAT::SkinnedMesh;
	meshDesc_5.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_5.vertexCount = wereWolf.meshInfo[0].vertices.size();
	meshDesc_5.pVertices = wereWolf.meshInfo[0].vertices.data();
	for (int i = 0; i < wereWolf.meshInfo[0].indices.size(); ++i)
	{
		meshDesc_5.meshSubsets.emplace_back(i,sizeof(uint16_t), wereWolf.meshInfo[0].indices[i].size(), wereWolf.meshInfo[0].indices[i].data());
	}
	unsigned long long meshTag_5 = 5;
	CreateMesh(meshDesc_5, meshTag_5);
}

IMesh* MeshManager::CreateMesh(const MeshDesc& desc, unsigned long long meshTag)
{
	IMesh* pMesh = Renderer::GetFactory()->CreateMesh(desc);
	meshTable_.Insert(pMesh, &meshTag, 8);
	return pMesh;
}

bool MeshManager::GetMesh(IMesh** ppOutMesh, unsigned long long meshTag)
{
	unsigned int searchedCount = 0;
	void* pTmpMesh = nullptr;

	if (false == meshTable_.Search((void**)&pTmpMesh, &searchedCount, 8, &meshTag, 8))
	{
		DEBUG_BREAK();
		//Assert("Mesh Search is Fail!!");
		return false;
	}

	if (searchedCount != 1)
	{
		DEBUG_BREAK();
		return false;
	}
	
	*ppOutMesh = static_cast<IMesh*>(pTmpMesh);
	return true;
}


void MeshManager::CleanUp()
{
	HashTableIterator meshTableEnd = meshTable_.end();
	for (HashTableIterator iter = meshTable_.begin(); iter != meshTableEnd;)
	{
		IMesh* pCurMesh = static_cast<IMesh*>(*iter);
		if (nullptr != pCurMesh)
		{
			pCurMesh->Release();
			pCurMesh = nullptr;
		}
		iter = meshTable_.Delete(iter);
	}
}