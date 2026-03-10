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
	//screenQuadDesc.vertexFormat = E_VERTEX_FORMAT::ScreenQuad;
	screenQuadDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	screenQuadDesc.usage = E_MESH_USAGE::DEFAULT;
	screenQuadDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	screenQuadDesc.vertexFormatSize = sizeof(ScreenQuadVertex);
	screenQuadDesc.vertexCount = screenQuadVertices.size();
	screenQuadDesc.pVertices = screenQuadVertices.data();
	screenQuadDesc.meshSubsets.emplace_back(0, (uint16_t)sizeof(WORD), screenQuadIndices.size(), screenQuadIndices.data());
	CreateMesh(screenQuadDesc, 0);




	std::vector<SimpleVertex> cubeVertices;
	std::vector<WORD> cubeIndices;
	GeometryGenerator::CreateCube(&cubeVertices, &cubeIndices);
	MeshDesc meshDesc_1;
	//meshDesc_1.vertexFormat = E_VERTEX_FORMAT::Simple;
	meshDesc_1.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	meshDesc_1.usage = E_MESH_USAGE::DEFAULT;
	meshDesc_1.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	meshDesc_1.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_1.vertexCount = cubeVertices.size();
	meshDesc_1.pVertices = cubeVertices.data();
	meshDesc_1.meshSubsets.emplace_back(0, (uint16_t)sizeof(WORD), cubeIndices.size(), cubeIndices.data());
	CreateMesh(meshDesc_1, 10);


	std::vector<SimpleVertex> sphereVertices;
	std::vector<WORD> sphereIndices;
	GeometryGenerator::CreateSphere(&sphereVertices, &sphereIndices);
	MeshDesc meshDesc_2;
	meshDesc_2.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	meshDesc_2.usage = E_MESH_USAGE::DEFAULT;
	meshDesc_2.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	//meshDesc_2.vertexFormat = E_VERTEX_FORMAT::Simple;
	meshDesc_2.vertexFormatSize = sizeof(SimpleVertex);
	meshDesc_2.vertexCount = sphereVertices.size();
	meshDesc_2.pVertices = sphereVertices.data();
	meshDesc_2.meshSubsets.emplace_back(0, (uint16_t)sizeof(WORD), sphereIndices.size(), sphereIndices.data());
	CreateMesh(meshDesc_2, 11);
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