#include "stdafx.h"
#include "Level.h"
#include "MeshComponent.h"
#include "StaticMeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "ActorPicker.h"

auto MakeTriangle = [&](void* pVertices, E_VERTEX_TYPE type,
	uint32_t idxA, uint32_t idxB, uint32_t idxC) -> Triangle
	{
		Triangle tri{};

		switch (type)
		{
		case E_VERTEX_TYPE::SIMPLE_VERTEX:
		{
			SimpleVertex* v = static_cast<SimpleVertex*>(pVertices);
			tri.a_ = v[idxA].position;
			tri.b_ = v[idxB].position;
			tri.c_ = v[idxC].position;
			break;
		}
		case E_VERTEX_TYPE::SKINNED_MESH:
		{
			SkinnedMeshVertex* v = static_cast<SkinnedMeshVertex*>(pVertices);
			tri.a_ = v[idxA].position;
			tri.b_ = v[idxB].position;
			tri.c_ = v[idxC].position;
			break;
		}
		default:
			break;
		}

		return tri;
	};

ActorPicker::ActorPicker()
	: pPickedActor_(nullptr),
	curPickedActorDiff_(0.0f)
{
}

ActorPicker::~ActorPicker()
{
	CleanUp();
}

ActorPicker* ActorPicker::GetInstance()
{
	return GActorPicker;
}

void ActorPicker::Tick(double deltaTime)
{
	if (Editor::GetEditor()->IsGizmoHovered()) return;
	
	if (InputManager::IsDown(VK_LBUTTON))
	{
		if (Editor::GetEditor()->IsWindowHovered("Scene"))
		{
			Float2 curMousePos = Editor::GetEditor()->GetMousePos();
			Float2 viewPortSize = Editor::GetEditor()->GetViewPortSize();

			Ray ray;
			ScreenToWorldRay(&ray, curMousePos, viewPortSize);
			TryPickObject(ray);
			Editor::GetEditor()->BindPickedActor(pPickedActor_);

			// Draw
			Float3 rayPos = { ray.origin_.X, ray.origin_.Y, ray.origin_.Z };
			Float3 rayDir = { ray.dir_.X, ray.dir_.Y, ray.dir_.Z };
			MATH::VectorNormalize(rayDir, rayDir);
			Debugger::DrawDebugRay(rayPos, rayDir, CameraManager::GetCurrentCamera()->GetFar(), { 1.0f, 0.0f, 0.0f, 1.0f });
		}
	}
}

void ActorPicker::ScreenToWorldRay(Ray* pOutRay, const Float2& screenPos, const Float2& viewPortSize)
{
	Float4x4 invProjection;
	MATH::MatrixInverse(invProjection, CameraManager::GetCurrentCamera()->GetProjectionMatrix());

	Float4x4 invView;
	MATH::MatrixInverse(invView, CameraManager::GetCurrentCamera()->GetViewMatrix());

	float ndc_x = (2.0f * screenPos.X) / viewPortSize.X - 1.0f;
	float ndc_y = 1.0f - (2.0f * screenPos.Y) / viewPortSize.Y;
	float ndc_z = 0.0f;
	float ndc_w = 1.0f;
	Float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };

	Float4 clickPos_CamSpace;
	MATH::MatrixMultiply(clickPos_CamSpace, ndcPos, invProjection);
	MATH::VectorScale(clickPos_CamSpace, clickPos_CamSpace, 1 / clickPos_CamSpace.W);

	Float4 clickPos_WorldSpace;
	MATH::MatrixMultiply(clickPos_WorldSpace, clickPos_CamSpace, invView);
	MATH::VectorScale(clickPos_WorldSpace, clickPos_WorldSpace, 1 / clickPos_WorldSpace.W);

	Float4 camPos_WorldSpace = CameraManager::GetCurrentCamera()->GetWorldTransform().GetPosition();
	Float4 rayDir = clickPos_WorldSpace - camPos_WorldSpace;
	rayDir.W = 0;
	MATH::VectorNormalize(rayDir, rayDir);

	pOutRay->origin_ = clickPos_WorldSpace;
	pOutRay->dir_ = rayDir;
	pOutRay->maxDistance_ = CameraManager::GetCurrentCamera()->GetFar();
}

bool ActorPicker::TryPickObject(const Ray& ray)
{
	pPickedActor_ = nullptr;
	curPickedActorDiff_ = FLT_MAX;

	Level* pCurLevel = World::Instance()->GetLevel();
	LinkedList* pActorList = pCurLevel->GetActorList(E_ACTOR_TYPE::NORMAL);
	LINK_NODE* pActorIter = pActorList->GetHead();
	while (pActorIter)
	{
		Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);

		Float4x4 invWorldMat;
		MATH::MatrixInverse(invWorldMat, pActor->GetWorldTransform().GetMatrix());

		Float4 rayOrigin_ObjSpace;
		MATH::MatrixMultiply(rayOrigin_ObjSpace, ray.origin_, invWorldMat);
		Float3 rayOrigin_ObjSpace_V3 = { rayOrigin_ObjSpace.X, rayOrigin_ObjSpace.Y, rayOrigin_ObjSpace.Z };

		Float4 rayDir_ObjSpace;
		MATH::MatrixMultiply(rayDir_ObjSpace, ray.dir_, invWorldMat);
		Float3 rayDir_ObjSpace_V3 = { rayDir_ObjSpace.X, rayDir_ObjSpace.Y, rayDir_ObjSpace.Z };
		MATH::VectorNormalize(rayDir_ObjSpace_V3, rayDir_ObjSpace_V3);

		// ObjectSapce Ray
		Ray ray_ObjSpace;
		ray_ObjSpace.origin_ = rayOrigin_ObjSpace;
		ray_ObjSpace.dir_ = rayDir_ObjSpace;
		ray_ObjSpace.maxDistance_ = ray.maxDistance_;


		float diffToAABB;
		if (RaycastBroadPhase(&diffToAABB, ray_ObjSpace, pActor))
		{
			if (curPickedActorDiff_ > diffToAABB)
			{
				float diffToTriangle;
				if (RaycastNarrowPhase(&diffToTriangle, ray_ObjSpace, pActor))
				{
					if (curPickedActorDiff_ > diffToTriangle)
					{
						curPickedActorDiff_ = diffToTriangle;
						pPickedActor_ = pActor;
					}
				}
			}
		}
		pActorIter = pActorIter->next_;
	}

	// TODO : 에디터모드에서는 Light도 피킹기능이 활성화. (수정해야 하는 코드, 하드 코딩)
	pActorList = pCurLevel->GetActorList(E_ACTOR_TYPE::LIGHT);
	pActorIter = pActorList->GetHead();
	while (pActorIter)
	{
		Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);

		Float4x4 invWorldMat;
		MATH::MatrixInverse(invWorldMat, pActor->GetWorldTransform().GetMatrix());

		Float4 rayOrigin_ObjSpace;
		MATH::MatrixMultiply(rayOrigin_ObjSpace, ray.origin_, invWorldMat);
		Float3 rayOrigin_ObjSpace_V3 = { rayOrigin_ObjSpace.X, rayOrigin_ObjSpace.Y, rayOrigin_ObjSpace.Z };

		Float4 rayDir_ObjSpace;
		MATH::MatrixMultiply(rayDir_ObjSpace, ray.dir_, invWorldMat);
		Float3 rayDir_ObjSpace_V3 = { rayDir_ObjSpace.X, rayDir_ObjSpace.Y, rayDir_ObjSpace.Z };
		MATH::VectorNormalize(rayDir_ObjSpace_V3, rayDir_ObjSpace_V3);

		// ObjectSapce Ray
		Ray ray_ObjSpace;
		ray_ObjSpace.origin_ = rayOrigin_ObjSpace;
		ray_ObjSpace.dir_ = rayDir_ObjSpace;
		ray_ObjSpace.maxDistance_ = ray.maxDistance_;

		float diffToAABB;
		if (RaycastBroadPhase(&diffToAABB, ray_ObjSpace, pActor))
		{
			if (curPickedActorDiff_ > diffToAABB)
			{
				float diffToTriangle;
				if (RaycastNarrowPhase(&diffToTriangle, ray_ObjSpace, pActor))
				{
					if (curPickedActorDiff_ > diffToTriangle)
					{
						curPickedActorDiff_ = diffToTriangle;
						pPickedActor_ = pActor;
					}
				}
			}
		}
		pActorIter = pActorIter->next_;
	}
	return nullptr != pPickedActor_;
}

bool ActorPicker::RaycastBroadPhase(float* pOutDistance, const Ray& ray, Actor* pActor)
{
	MeshComponent* pStaticMeshComponent = pActor->GetComponent<StaticMeshComponent>();
	if (nullptr != pStaticMeshComponent)
	{
		IPSO* pPSO = pStaticMeshComponent->GetPSO();
		uint32_t meshCnt = pPSO->GetMeshSlotCount();
		for (int meshIdx = 0; meshIdx < meshCnt; ++meshIdx)
		{
			AABB aabb = pPSO->GetMesh(meshIdx)->GetAABB();
			return MATH::Intersection3D_Ray_AABB(pOutDistance, ray, aabb);
		}
	}
	MeshComponent* pSkeletalMeshComponent = pActor->GetComponent<SkeletalMeshComponent>();
	if (nullptr != pSkeletalMeshComponent)
	{
		IPSO* pPSO = pSkeletalMeshComponent->GetPSO();
		uint32_t meshCnt = pPSO->GetMeshSlotCount();
		for (int meshIdx = 0; meshIdx < meshCnt; ++meshIdx)
		{
			AABB aabb = pPSO->GetMesh(meshIdx)->GetAABB();
			return MATH::Intersection3D_Ray_AABB(pOutDistance, ray, aabb);
		}
	}
	return false;
}

bool ActorPicker::RaycastNarrowPhase(float* pOutDistance, const Ray& ray, Actor* pActor)
{
	float minDistance = FLT_MAX;
	bool finalRet = false;

	MeshComponent* pStaticMeshComponent = pActor->GetComponent<StaticMeshComponent>();
	if (nullptr != pStaticMeshComponent)
	{
		IPSO* pPSO = pStaticMeshComponent->GetPSO();
		uint32_t meshCnt = pPSO->GetMeshSlotCount();
		for (int meshIdx = 0; meshIdx < meshCnt; ++meshIdx)
		{
			IMesh* pMesh = pPSO->GetMesh(meshIdx);

			void* pVertices = nullptr;
			E_VERTEX_TYPE vertexType;
			pMesh->GetVerticesData(&pVertices, &vertexType);

			for (int subMesh = 0; subMesh < pMesh->GetSubMeshCount(); ++subMesh)
			{
				uint32_t* pIndices = nullptr;
				uint32_t indicesCount = 0;
				pMesh->GetIndiciesData((void**)&pIndices, &indicesCount, subMesh);

				for (int index = 0; index < indicesCount; index += 3)
				{
					uint16_t idxA = pIndices[index];
					uint16_t idxB = pIndices[index + 1];
					uint16_t idxC = pIndices[index + 2];

					Triangle triangle = MakeTriangle(pVertices, vertexType, idxA, idxB, idxC);
					float distance;
					if (MATH::Intersection3D_Ray_Triangle(&distance, ray, triangle))
					{
						minDistance = min(minDistance, distance);
						finalRet = true;
					}
				}

			}
		}
	}

	MeshComponent* pSkeletalMeshComponent = pActor->GetComponent<SkeletalMeshComponent>();
	if (nullptr != pSkeletalMeshComponent)
	{
		IPSO* pPSO = pSkeletalMeshComponent->GetPSO();
		uint32_t meshCnt = pPSO->GetMeshSlotCount();
		for (int meshIdx = 0; meshIdx < meshCnt; ++meshIdx)
		{
			IMesh* pMesh = pPSO->GetMesh(meshIdx);

			void* pVertices = nullptr;
			E_VERTEX_TYPE vertexType;
			pMesh->GetVerticesData(&pVertices, &vertexType);

			for (int subMesh = 0; subMesh < pMesh->GetSubMeshCount(); ++subMesh)
			{
				uint32_t* pIndices = nullptr;
				uint32_t indicesCount = 0;
				pMesh->GetIndiciesData((void**)&pIndices, &indicesCount, subMesh);

				for (int index = 0; index < indicesCount; index += 3)
				{
					uint16_t idxA = pIndices[index];
					uint16_t idxB = pIndices[index + 1];
					uint16_t idxC = pIndices[index + 2];

					Triangle triangle = MakeTriangle(pVertices, vertexType, idxA, idxB, idxC);
					float distance;
					if (MATH::Intersection3D_Ray_Triangle(&distance, ray, triangle))
					{
						minDistance = min(minDistance, distance);
						finalRet = true;
					}
				}
			}
		}
	}

	if (finalRet)
	{
		*pOutDistance = minDistance;
		return true;
	}

	return false;
}

Actor* ActorPicker::GetPickedActor() const
{
	return GActorPicker->pPickedActor_;
}

void ActorPicker::CleanUp()
{

}
