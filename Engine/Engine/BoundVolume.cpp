#include "stdafx.h"
#include "RenderComponent.h"
#include "BoundVolume.h"

BoundVolume::BoundVolume()
	:pMesh_(nullptr),
	boundVolume_()
{
}

BoundVolume::~BoundVolume()
{
	CleanUp();
}

bool BoundVolume::Init(RenderComponent* pRenderComponent)
{
	if (nullptr == pRenderComponent)
	{
		DEBUG_BREAK();
		return false;
	}

	pRenderComponent->BindOnMeshLoaded(std::bind(&BoundVolume::UpdateBoundVolume, this, std::placeholders::_1));

	return true;
}

void BoundVolume::UpdateBoundVolume(IMesh* pMesh)
{
	if (nullptr == pMesh)
	{
		Assert("Mesh == NULL!!");
		return;
	}
	pMesh_ = pMesh;
	pMesh_->AddRef();

	E_VERTEX_FORMAT vertexFormat;
	uint32_t vertexStride;
	uint32_t vertexCount;
	void* pVertices = nullptr;
	pMesh->GetVerticesData(&vertexFormat, &vertexStride, &vertexCount, &pVertices);

	BoundVolume_AABB boundVolume;
	if (false == (CalculateBoundVolume(&boundVolume, vertexFormat, vertexStride, vertexCount, pVertices)))
	{
		Assert("CalculateBoundVolume == False!!");
		return;
	}

	boundVolume_ = boundVolume;
}

bool BoundVolume::CalculateBoundVolume
(
	BoundVolume_AABB* pOutBoundVolume, 
	E_VERTEX_FORMAT vertexFormat, 
	uint32_t vertexStride, 
	uint32_t vertexCount,
	void* pVertices
)
{
	if (nullptr == pVertices)
	{
		Assert("Vertices == NULL");
		return false;
	}

	BoundVolume_AABB resultAABB;
	switch (vertexFormat)
	{
	case E_VERTEX_FORMAT::SIMPLE:
	{
		SimpleVertex* pSimpleVertices = static_cast<SimpleVertex*>(pVertices);
		resultAABB.minPos = pSimpleVertices[0].position;
		resultAABB.maxPos = pSimpleVertices[0].position;
		for (int i = 1; i < vertexCount; ++i)
		{
			const SimpleVertex& curVertex = pSimpleVertices[i];
			resultAABB.minPos.X = min(resultAABB.minPos.X, curVertex.position.X);
			resultAABB.maxPos.X = max(resultAABB.maxPos.X, curVertex.position.X);

			resultAABB.minPos.Y = min(resultAABB.minPos.Y, curVertex.position.Y);
			resultAABB.maxPos.Y = max(resultAABB.maxPos.Y, curVertex.position.Y);

			resultAABB.minPos.Z = min(resultAABB.minPos.Z, curVertex.position.Z);
			resultAABB.maxPos.Z = max(resultAABB.maxPos.Z, curVertex.position.Z);
		}
	}break;
	case E_VERTEX_FORMAT::SCREEN_RECT:
	{
		ScreenRectVertex* pScreenRectVertices = static_cast<ScreenRectVertex*>(pVertices);
		resultAABB.minPos = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		resultAABB.maxPos = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		for (int i = 1; i < vertexCount; ++i)
		{
			const ScreenRectVertex& curVertex = pScreenRectVertices[i];
			resultAABB.minPos.X = min(resultAABB.minPos.X, curVertex.position.X);
			resultAABB.maxPos.X = max(resultAABB.maxPos.X, curVertex.position.X);

			resultAABB.minPos.Y = min(resultAABB.minPos.Y, curVertex.position.Y);
			resultAABB.maxPos.Y = max(resultAABB.maxPos.Y, curVertex.position.Y);
		}
	}break;
	default:
		break;
	}

	*pOutBoundVolume = resultAABB;
	return true;
}


void BoundVolume::CleanUp()
{
	if (nullptr != pMesh_)
	{
		pMesh_->Release();
		pMesh_ = nullptr;
	}
}