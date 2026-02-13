#include "stdafx.h"
#include "RenderComponent.h"
#include "BoundVolume.h"

BoundVolume::BoundVolume()
	:pMesh_(nullptr),
	aabb_()
{
}

BoundVolume::~BoundVolume()
{
	CleanUp();
}

bool BoundVolume::Init(RenderComponent* pRenderComponent, Transform* pTransform)
{
	if (nullptr == pRenderComponent)
	{
		DEBUG_BREAK();
		return false;
	}
	if (nullptr == pTransform)
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

	AABB aabb;
	if (false == (CalculateAABB(&aabb, vertexFormat, vertexStride, vertexCount, pVertices)))
	{
		Assert("CalculateBoundVolume == False!!");
		return;
	}

	aabb_ = aabb;
}

const AABB& BoundVolume::GetAABB() const
{
	return aabb_;
}

bool BoundVolume::CalculateAABB
(
	AABB* pOutAABB,
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

	AABB resultAABB;
	switch (vertexFormat)
	{
	case E_VERTEX_FORMAT::ScreenQuad:
	{
		ScreenQuadVertex* pScreenRectVertices = static_cast<ScreenQuadVertex*>(pVertices);
		resultAABB.minPos_ = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		resultAABB.maxPos_ = { pScreenRectVertices[0].position.X, pScreenRectVertices[0].position.Y, 0.0f };
		for (int i = 1; i < vertexCount; ++i)
		{
			const ScreenQuadVertex& curVertex = pScreenRectVertices[i];
			resultAABB.minPos_.X = min(resultAABB.minPos_.X, curVertex.position.X);
			resultAABB.maxPos_.X = max(resultAABB.maxPos_.X, curVertex.position.X);

			resultAABB.minPos_.Y = min(resultAABB.minPos_.Y, curVertex.position.Y);
			resultAABB.maxPos_.Y = max(resultAABB.maxPos_.Y, curVertex.position.Y);
		}
		break;
	}
	case E_VERTEX_FORMAT::Simple:
	{
		SimpleVertex* pSimpleVertices = static_cast<SimpleVertex*>(pVertices);
		resultAABB.minPos_ = pSimpleVertices[0].position;
		resultAABB.maxPos_ = pSimpleVertices[0].position;
		for (int i = 1; i < vertexCount; ++i)
		{
			const SimpleVertex& curVertex = pSimpleVertices[i];
			resultAABB.minPos_.X = min(resultAABB.minPos_.X, curVertex.position.X);
			resultAABB.maxPos_.X = max(resultAABB.maxPos_.X, curVertex.position.X);

			resultAABB.minPos_.Y = min(resultAABB.minPos_.Y, curVertex.position.Y);
			resultAABB.maxPos_.Y = max(resultAABB.maxPos_.Y, curVertex.position.Y);

			resultAABB.minPos_.Z = min(resultAABB.minPos_.Z, curVertex.position.Z);
			resultAABB.maxPos_.Z = max(resultAABB.maxPos_.Z, curVertex.position.Z);
		}
		break;
	};
	case E_VERTEX_FORMAT::SkinnedMesh:
	{
		SkinnedMeshVertex* pSkinnedVertices = static_cast<SkinnedMeshVertex*>(pVertices);
		resultAABB.minPos_ = pSkinnedVertices[0].position;
		resultAABB.maxPos_ = pSkinnedVertices[0].position;
		for (int i = 1; i < vertexCount; ++i)
		{
			const SkinnedMeshVertex& curVertex = pSkinnedVertices[i];
			resultAABB.minPos_.X = min(resultAABB.minPos_.X, curVertex.position.X);
			resultAABB.maxPos_.X = max(resultAABB.maxPos_.X, curVertex.position.X);

			resultAABB.minPos_.Y = min(resultAABB.minPos_.Y, curVertex.position.Y);
			resultAABB.maxPos_.Y = max(resultAABB.maxPos_.Y, curVertex.position.Y);

			resultAABB.minPos_.Z = min(resultAABB.minPos_.Z, curVertex.position.Z);
			resultAABB.maxPos_.Z = max(resultAABB.maxPos_.Z, curVertex.position.Z);
		}
		break;
	}
	default:
		DEBUG_BREAK();
		break;
	}

	*pOutAABB = resultAABB;
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