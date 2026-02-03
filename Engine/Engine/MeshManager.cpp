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
	std::vector<SimpleVertex> cubeVertices;
	std::vector<WORD> cubeIndices;
	GeometryGenerator::CreateCube(&cubeVertices, &cubeIndices);
	MeshDesc meshDesc_1;
	meshDesc_1.vertexFormat = E_VERTEX_FORMAT::SIMPLE;
	meshDesc_1.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_1.vertexCount = cubeVertices.size();
	meshDesc_1.vertices = cubeVertices.data();
	meshDesc_1.indexFormatSize = sizeof(WORD);
	meshDesc_1.indexCount = cubeIndices.size();
	meshDesc_1.indices = cubeIndices.data();
	unsigned long long meshTag_1 = 1;
	CreateMesh(meshDesc_1, meshTag_1);


	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;
	GeometryGenerator::CreateSphere(&sphereVertices, &sphereIndices);
	MeshDesc meshDesc_2;
	meshDesc_2.vertexFormat = E_VERTEX_FORMAT::SIMPLE;
	meshDesc_2.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_2.vertexCount = sphereVertices.size();
	meshDesc_2.vertices = sphereVertices.data();
	meshDesc_2.indexFormatSize = sizeof(WORD); 
	meshDesc_2.indexCount = sphereIndices.size();
	meshDesc_2.indices = sphereIndices.data();
	unsigned long long meshTag_2 = 2;
	CreateMesh(meshDesc_2, meshTag_2);


	Model wukong;
	FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong.FBX");
	//FBXManager::LoadModel(&wukong, "..\\Resource\\Fbx\\Wukong\\Wukong_Fury.FBX");
	MeshDesc meshDesc_3;
	meshDesc_3.vertexFormat = E_VERTEX_FORMAT::SIMPLE;
	meshDesc_3.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_3.vertexCount = wukong.vertices_.size();
	meshDesc_3.vertices = wukong.vertices_.data();
	meshDesc_3.indexFormatSize = sizeof(uint16_t);
	meshDesc_3.indexCount = wukong.indices_.size();
	meshDesc_3.indices = wukong.indices_.data();
	unsigned long long meshTag_3 = 3;
	CreateMesh(meshDesc_3, meshTag_3);


	Model capoeira;
	FBXManager::LoadModel(&capoeira, "..\\Resource\\Fbx\\Mixamo\\Capoeira.FBX");
	MeshDesc meshDesc_4;
	meshDesc_4.vertexFormat = E_VERTEX_FORMAT::SIMPLE;
	meshDesc_4.vertexFormatSize = sizeof(SkinnedMeshVertex);
	meshDesc_4.vertexCount = capoeira.vertices_.size();
	meshDesc_4.vertices = capoeira.vertices_.data();
	meshDesc_4.indexFormatSize = sizeof(uint16_t);
	meshDesc_4.indexCount = capoeira.indices_.size();
	meshDesc_4.indices = capoeira.indices_.data();
	unsigned long long meshTag_4 = 4;
	CreateMesh(meshDesc_4, meshTag_4);
}

IMesh* MeshManager::CreateMesh(const MeshDesc& desc, unsigned long long meshTag)
{
	IMesh* pMesh = GRenderer->CreateMesh(desc);
	//pMesh->AddInputLayout("POSITION", 0, 6, 0, false);
	//pMesh->AddInputLayout("COLOR", 0, 2, 0, false);
	//pMesh->AddInputLayout("NORMAL", 0, 6, 0, false);
	//pMesh->AddInputLayout("TEXCOORD", 0, 16, 0, false);
	//pMesh->AddInputLayout("TANGENT", 0, 2, 0, false);

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