#include "stdafx.h"
#include "Level.h"
#include "BoundVolume.h"
#include "RenderComponent.h"
#include "ActorPicker.h"



ActorPicker::ActorPicker()
	:pPickedActor_(nullptr),
	curPickedActorDiff_(0.0f),
	pickedMousePos_({ 0.0f, 0.0f })
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
	//if (pPickedActor_ != nullptr) return;

	if (InputManager::Instance()->IsDown(VK_LBUTTON))
	{
		pickedMousePos_ = InputManager::Instance()->GetCurrentMousePosition();

		Ray ray;
		ScreenToWorldRay(&ray, pickedMousePos_);

		TryPickObject(ray);

		ImGuiSystem::GetImGuiManager()->BindPickedActor(pPickedActor_);


		// DEBUG
		Float3 rayPos = { ray.origin_.X, ray.origin_.Y, ray.origin_.Z };
		Float3 rayDir = { ray.dir_.X, ray.dir_.Y, ray.dir_.Z };
		MATH::VectorNormalize(rayDir, rayDir);
		GDebugRenderer->DrawRay(rayPos, rayDir, GCurrentCamera->GetFar(), { 1.0f, 0.0f, 0.0f, 1.0f });
	}
}


void ActorPicker::ScreenToWorldRay(Ray* pOutRay, const Float2& screenPos)
{
	Float4x4 invProjection;
	MATH::MatrixInverse(invProjection, GCurrentCamera->Projection());

	Float4x4 invView;
	MATH::MatrixInverse(invView, GCurrentCamera->View());

	float ndc_x = (2.0f * screenPos.X) / DEFAULT_SCREEN_WIDTH - 1.0f;
	float ndc_y = 1.0f - (2.0f * screenPos.Y) / DEFAULT_SCREEN_HEIGHT;
	float ndc_z = 0.0f;
	float ndc_w = 1.0f;
	Float4 ndcPos = { ndc_x, ndc_y, ndc_z, ndc_w };

	Float4 clickPos_CamSpace;
	MATH::MatrixMultiply(clickPos_CamSpace, ndcPos, invProjection);
	MATH::VectorScale(clickPos_CamSpace, clickPos_CamSpace, 1 / clickPos_CamSpace.W);

	Float4 clickPos_WorldSpace;
	MATH::MatrixMultiply(clickPos_WorldSpace, clickPos_CamSpace, invView);
	MATH::VectorScale(clickPos_WorldSpace, clickPos_WorldSpace, 1 / clickPos_WorldSpace.W);

	Float4 camPos_WorldSpace = GCurrentCamera->GetWorldTransform().GetPosition();
	Float4 rayDir = clickPos_WorldSpace - camPos_WorldSpace;
	rayDir.W = 0;
	MATH::VectorNormalize(rayDir, rayDir);

	pOutRay->origin_ = clickPos_WorldSpace;
	pOutRay->dir_ = rayDir;
	pOutRay->maxDistance_ = GCurrentCamera->GetFar();	
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
		MATH::MatrixInverse(invWorldMat, pActor->GetWorldTransform().GetWorldMatrix());

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


		float diffToVolume;
		if (RaycastBroadPhase(&diffToVolume, ray_ObjSpace, pActor))
		{
			if (curPickedActorDiff_ > diffToVolume)
			{
				float diffToVertex;
				if (RaycastNarrowPhase(&diffToVertex, ray_ObjSpace, pActor))
				{
					if (curPickedActorDiff_ > diffToVertex)
					{
						curPickedActorDiff_ = diffToVertex;
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
	// AABB
	BoundVolume* pBoundVolume = pActor->GetBoundVolume();
	const AABB& aabb = pBoundVolume->GetAABB();

	return MATH::Intersection3D_Ray_AABB(pOutDistance, ray, aabb);
}

bool ActorPicker::RaycastNarrowPhase(float* pOutDistance, const Ray& ray, Actor* pActor)
{
	RenderComponent* pRenderer = pActor->GetRenderComponent();
	IMesh* pMesh = pRenderer->GetMesh();

	E_VERTEX_FORMAT vertexFormat;
	uint32_t vertexStride;
	uint32_t vertexCount;
	void* pVertices;
	pMesh->GetVerticesData(&vertexFormat, &vertexStride, &vertexCount, &pVertices);

	uint32_t indexStride;
	uint32_t indexCount;
	void* pIndices;
	pMesh->GetIndicesData(&indexStride, &indexCount, &pIndices);


	SimpleVertex* pVerticesSimple = static_cast<SimpleVertex*>(pVertices);
	WORD* pIndicesWORD = static_cast<WORD*>(pIndices);


	float minDistance = FLT_MAX;
	bool finalRet = false;
	for (int i = 0; i < indexCount; i += 3)
	{
		WORD idxA = pIndicesWORD[i];
		WORD idxB = pIndicesWORD[i + 1];
		WORD idxC = pIndicesWORD[i + 2];

		const SimpleVertex& vertexA = pVerticesSimple[idxA];
		const SimpleVertex& vertexB = pVerticesSimple[idxB];
		const SimpleVertex& vertexC = pVerticesSimple[idxC];

		Triangle triangle;
		triangle.a_ = vertexA.position;
		triangle.b_ = vertexB.position;
		triangle.c_ = vertexC.position;

		float distance;
		if(MATH::Intersection3D_Ray_Triangle(&distance, ray, triangle))
		{
			minDistance = min(minDistance, distance);
			finalRet = true;
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




