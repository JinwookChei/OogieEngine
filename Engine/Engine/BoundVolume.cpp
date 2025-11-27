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

bool BoundVolume::IntersectionRayAABB(float* pOutT, const Float3& origin, const Float3& dir)
{
	// X축 슬랩(평면)에 대한 진입/진출 t 값 계산
	float t1 = (boundVolume_.minPos.X - origin.X) / dir.X;
	float t2 = (boundVolume_.maxPos.X - origin.X) / dir.X;
	// tmin은 항상 작은 값, tmax는 항상 큰 값이 되도록 설정
	float tMin = min(t1, t2);
	float tMax = max(t1, t2);

	// Y축 슬랩 검사
	float t3 = (boundVolume_.minPos.Y - origin.Y) / dir.Y;
	float t4 = (boundVolume_.maxPos.Y - origin.Y) / dir.Y;
	tMin = max(tMin, min(t3, t4));
	tMax = min(tMax, max(t3, t4));

	// Z축 슬랩 검사
	float t5 = (boundVolume_.minPos.Z - origin.Z) / dir.Z;
	float t6 = (boundVolume_.maxPos.Z - origin.Z) / dir.Z;
	tMin = max(tMin, min(t5, t6));
	tMax = min(tMax, max(t5, t6));

	// 충돌 조건 확인
	// 1. tMax < tMin: 슬랩들의 교차 구간이 없음 (빗나감)
	// 2. tMax < 0: AABB가 레이의 뒤쪽에 있음
	if (tMax < tMin || tMax < 0.0f) {
		return false;
	}
	
	// 충돌 발생. tOut에 가장 가까운 충돌 거리 저장
	// tMin이 0보다 작으면(레이 시작점이 박스 내부) tMax가 출구, 아니면 tMin이 입구
	if (tMin < 0.0f)
	{
		*pOutT = tMax;
	}
	else
	{
		*pOutT = tMin;
	}

	return true;
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