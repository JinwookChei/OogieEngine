#include "stdafx.h"
#include "Level.h"
#include "BoundVolume.h"
#include "ObjectPicker.h"


ObjectPicker::ObjectPicker()
	:pPickedActor_(nullptr),
	curPickedActorDiff_(0.0f),
	pickedMousePos_({ 0.0f, 0.0f })
{
}

ObjectPicker::~ObjectPicker()
{
	CleanUp();
}

ObjectPicker* ObjectPicker::GetInstance()
{
	return GObjectPicker;
}

void ObjectPicker::Tick(double deltaTime)
{
	if (InputManager::Instance()->IsDown(VK_LBUTTON))
	{
		pickedMousePos_ = InputManager::Instance()->GetCurrentMousePosition();

		RayCastFromScreen(pickedMousePos_);
	}
}

void ObjectPicker::RayCastFromScreen(const Float2& screenPos)
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

	// Debug Ray
	Float3 rayStart = { camPos_WorldSpace.X, camPos_WorldSpace.Y, camPos_WorldSpace.Z };
	Float4 rayDir_V4 = clickPos_WorldSpace - camPos_WorldSpace;
	Float3 rayDir_V3 = { rayDir_V4.X, rayDir_V4.Y, rayDir_V4.Z };
	Float3 rayDirNorm;
	MATH::VectorNormalize(rayDirNorm, rayDir_V3);
	GDebugRenderer->DrawRay(rayStart, rayDirNorm, GCurrentCamera->GetFar(), { 1.0f, 0.0f, 0.0f, 1.0f });

	// Debug Ray End


	pPickedActor_ = nullptr;
	curPickedActorDiff_ = FLT_MAX;

	// Picking ÈÄº¸.
	Level* pCurLevel = World::Instance()->GetLevel();
	LinkedList* pActorList = pCurLevel->GetActorList(E_ACTOR_TYPE::NORMAL);
	LINK_NODE* pActorIter = pActorList->GetHead();
	while (pActorIter)
	{
		Actor* pActor = static_cast<Actor*>(pActorIter->pItem_);

		Float4x4 invWorldMat;
		MATH::MatrixInverse(invWorldMat, pActor->GetWorldTransform().GetWorldMatrix());

		Float4 camPos_ObjSpace;
		MATH::MatrixMultiply(camPos_ObjSpace, camPos_WorldSpace, invWorldMat);
		Float3 camPos_ObjSpace_V3 = { camPos_ObjSpace.X, camPos_ObjSpace.Y, camPos_ObjSpace.Z };

		Float4 rayDir_ObjSpace;
		MATH::MatrixMultiply(rayDir_ObjSpace, rayDir_V4, invWorldMat);
		Float3 rayDir_ObjSpace_V3 = { rayDir_ObjSpace.X, rayDir_ObjSpace.Y, rayDir_ObjSpace.Z };
		MATH::VectorNormalize(rayDir_ObjSpace_V3, rayDir_ObjSpace_V3);


		float diffVolume;
		if (true == pActor->GetBoundVolumeTEMP()->IntersectionRayAABB(&diffVolume, camPos_ObjSpace_V3, rayDir_ObjSpace_V3, GCurrentCamera->GetFar()))
		{
			if (curPickedActorDiff_ > diffVolume)
			{
				float diffVertex;
				if (pActor->TEMP_IntersectionRayTriangle(&diffVertex, camPos_ObjSpace_V3, rayDir_ObjSpace_V3))
				{
					if (curPickedActorDiff_ > diffVertex)
					{
						curPickedActorDiff_ = diffVertex;
						pPickedActor_ = pActor;
					}
				}
			}
		}

		pActorIter = pActorIter->next_;
	}
}

Actor* ObjectPicker::GetPickedActor() const
{
	return GObjectPicker->pPickedActor_;
}

void ObjectPicker::CleanUp()
{

}




