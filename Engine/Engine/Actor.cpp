#include "stdafx.h"
#include "RenderComponent.h"
#include "BoundVolume.h"
#include "Actor.h"



Actor::Actor()
	: pRenderer_(new RenderComponent(this)),
	pTransform_(new Transform),
	pBoundVolume(new BoundVolume)
{
	levelLink_.prev_ = nullptr;
	levelLink_.next_ = nullptr;
	levelLink_.pItem_ = this;

	pBoundVolume->Init(pRenderer_);
}

Actor::~Actor()
{
	CleanUp();
}

void Actor::Render()
{

}

Transform& Actor::GetWorldTransform() const
{
	return *pTransform_;
}

LINK_NODE* Actor::LevelLink()
{
	return &levelLink_;
}

BoundVolume* Actor::GetBoundVolumeTEMP() const
{
	return pBoundVolume;
}

bool Actor::TEMP_IntersectionRayTriangle(float* pOutT, const Float3& pos, const Float3& dir)
{

	IMesh* pMesh = pRenderer_->GetMesh();

	E_VERTEX_FORMAT vertexFormat;
	uint32_t vertexStride;
	uint32_t vertexCount;
	void* pVertices;
	pMesh->GetVerticesData(&vertexFormat, &vertexStride, &vertexCount, &pVertices);

	uint32_t indexStride;
	uint32_t indexCount;
	void* pIndices;
	pMesh->GetIndicesData(&indexStride, &indexCount, &pIndices);
	

	WORD* pIndicesWORD = static_cast<WORD*>(pIndices);
	SimpleVertex* pVerticesSimple = static_cast<SimpleVertex*>(pVertices);

	float minT = FLT_MAX;
	bool finalRet = false;
	for (int i = 0; i < indexCount; i += 3)
	{
		WORD idxA = pIndicesWORD[i];
		WORD idxB = pIndicesWORD[i + 1];
		WORD idxC = pIndicesWORD[i + 2];

		const SimpleVertex& vertexA = pVerticesSimple[idxA];
		const SimpleVertex& vertexB = pVerticesSimple[idxB];
		const SimpleVertex& vertexC = pVerticesSimple[idxC];

		float t;
		bool ret = MATH::Intersection3D_Ray_Triangle(&t, pos, dir, vertexA.position, vertexB.position, vertexC.position);
		if (ret)
		{
			minT = min(minT, t);
			finalRet = true;
		}
	}

	if (finalRet)
	{
		*pOutT = minT;
		return true;
	}

	return false;


	/*switch (vertexFormat)
	{
	case E_VERTEX_FORMAT::SIMPLE:
		break;
	case E_VERTEX_FORMAT::SCREEN_RECT:
		break;
	default:
		break;
	}*/
}

void Actor::CleanUp()
{
	if (nullptr != pBoundVolume)
	{
		delete pBoundVolume;
		pBoundVolume = nullptr;
	}
	if (nullptr != pTransform_)
	{
		delete pTransform_;
		pTransform_ = nullptr;
	}
	if (nullptr != pRenderer_)
	{
		delete pRenderer_;
		pRenderer_ = nullptr;
	}
}
