#include "stdafx.h"
#include "MeshManager.h"
#include "GeometryGenerator.h"

MeshManager::MeshManager()
	: meshTable_(16,16)
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
	std::vector<uint32_t> screenQuadIndices;
	GeometryGenerator::CreateScreenQuad(&screenQuadVertices, &screenQuadIndices);
	MeshDesc screenQuadDesc;
	screenQuadDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	screenQuadDesc.vertexType = E_VERTEX_TYPE::SCREEN_QUAD;
	screenQuadDesc.usage = E_MESH_USAGE::DEFAULT;
	screenQuadDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	screenQuadDesc.bufferSize = sizeof(ScreenQuadVertex) * screenQuadVertices.size();
	screenQuadDesc.vertexFormatSize = sizeof(ScreenQuadVertex);
	screenQuadDesc.vertexCount = screenQuadVertices.size();
	screenQuadDesc.pVertices = screenQuadVertices.data();
	screenQuadDesc.subMeshDesc.emplace_back(0, (uint32_t)screenQuadIndices.size(), screenQuadIndices.data());
	CreateMesh(screenQuadDesc, "MS_ScreenQuad", 13);

	std::vector<StaticMeshVertex> cubeVertices;
	std::vector<uint32_t> cubeIndices;
	GeometryGenerator::CreateCube(&cubeVertices, &cubeIndices);
	MeshDesc cubeDesc;
	cubeDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	cubeDesc.vertexType = E_VERTEX_TYPE::STATIC_MESH;
	cubeDesc.usage = E_MESH_USAGE::DEFAULT;
	cubeDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	cubeDesc.bufferSize = sizeof(StaticMeshVertex) * cubeVertices.size();
	cubeDesc.vertexFormatSize = sizeof(StaticMeshVertex);
	cubeDesc.vertexCount = cubeVertices.size();
	cubeDesc.pVertices = cubeVertices.data();
	cubeDesc.subMeshDesc.emplace_back(0, (uint32_t)cubeIndices.size(), cubeIndices.data());
	CreateMesh(cubeDesc, "MS_Cube", 7);

	std::vector<StaticMeshVertex> sphereVertices;
	std::vector<uint32_t> sphereIndices;
	GeometryGenerator::CreateSphere(&sphereVertices, &sphereIndices);
	MeshDesc sphereDesc;
	sphereDesc.primitiveType = E_MESH_PRIMITIVE_TYPE::TRIANGLE;
	sphereDesc.vertexType = E_VERTEX_TYPE::STATIC_MESH;
	sphereDesc.usage = E_MESH_USAGE::DEFAULT;
	sphereDesc.bindFlag = E_MESH_BIND_FLAG::VERTEX_BUFFER;
	sphereDesc.bufferSize = sizeof(StaticMeshVertex) * sphereVertices.size();
	sphereDesc.vertexFormatSize = sizeof(StaticMeshVertex);
	sphereDesc.vertexCount = sphereVertices.size();
	sphereDesc.pVertices = sphereVertices.data();
	sphereDesc.subMeshDesc.emplace_back(0, (uint32_t)sphereIndices.size(), sphereIndices.data());
	CreateMesh(sphereDesc, "MS_Sphere", 9);
}

IMesh* MeshManager::CreateMesh(const MeshDesc& desc, const char* meshKey, unsigned int keySize)
{
	IMesh* pMesh = Renderer::GetFactory()->CreateMesh(desc);
	//meshTable_.Insert(pMesh, &meshTag, keySize);
	meshTable_.Insert(pMesh, meshKey, keySize);

	return pMesh;
}

bool MeshManager::GetMesh(IMesh** ppOutMesh, const char* meshKey, unsigned int keySize)
{
	unsigned int searchedCount = 0;
	void* pTmpMesh = nullptr;
	if (false == meshTable_.Search((void**)&pTmpMesh, &searchedCount, 8, meshKey, keySize))
	{
		DEBUG_BREAK();
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